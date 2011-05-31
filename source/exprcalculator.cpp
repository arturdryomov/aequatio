#include "exprcalculator.h"

#include <QStack>

#define _USE_MATH_DEFINES
#include <qmath.h>

ExprCalculator::ExprCalculator(QObject *parent) : QObject(parent)
{
	initializeBuiltInFunctions();
	initializeBuiltInConstants();
}

Number ExprCalculator::calculate(const RpnCodeThread &thread)
{
	RpnFunction function = {0, thread};
	m_functions.insert(RpnFunctionMain, function);

	return calculateFunction(RpnFunctionMain, QList<Number>());
}

Number ExprCalculator::calculateFunction(QString functionName, QList<Number> functionArguments)
{
	QStack<Number> calculationStack;

	RpnCodeThread currentThread = m_functions.value(functionName).codeThread;

	// Find and extract

	foreach (RpnElement element, currentThread) {

		switch (element.type) {
			// Just push to numbers stack
			case RpnElementOperand:
				calculationStack.push(element.value.value<Number>());
				break;

			// Get number and push to stack
			case RpnElementParam:
				calculationStack.push(functionArguments[element.value.value<int>()]);
				break;

			// Find constant and push its value
			case RpnElementConstant:
				if (m_builtInConstants.contains(element.value.value<QString>())) {
					calculationStack.push(m_builtInConstants.value(element.value.value<QString>()));					
				}
				else if (m_constants.contains(element.value.value<QString>())) {
					calculationStack.push(m_constants.value(element.value.value<QString>()));	
				}
				else {
					throw Exception(tr("Unknown constant"));
				}
				
				break;

			// Find function and call it
			case RpnElementFunction: {
				QString calledFunctionName = element.value.value<QString>();

				if (m_builtInFunctions.contains(calledFunctionName)) {
					QList<Number> actualArguments;
					for (int i = 0; i < m_builtInFunctions.value(calledFunctionName); i++) {
						actualArguments.prepend(calculationStack.pop());
					}
					Number result = calculateBuiltInFunction(calledFunctionName, actualArguments);
					calculationStack.push(result);
				}

				else if (m_functions.contains(calledFunctionName)) {
					QList<Number> actualArguments;
					for (int i = 0; i < m_functions.value(calledFunctionName).argumentsCount; i++) {
						actualArguments.prepend(calculationStack.pop());
					}
					Number result = calculateFunction(calledFunctionName, actualArguments);
					calculationStack.push(result);
				}

				else {
					throw Exception(tr("Unknown function"));
				}
				break;
			}
			default:
				throw Exception(tr("Unknown RPN element"));
		}
	}

	if (calculationStack.count() != 1) {
		throw Exception(tr("There are too much elements in RPN stack"));
	}
	return calculationStack.pop();
}

Number ExprCalculator::calculateBuiltInFunction(QString functionName, QList<Number> functionArguments)
{
	if (functionName == RpnFunctionPlus) {
		return functionArguments[0] + functionArguments[1];
	} 
	else if (functionName == RpnFunctionMinus) {
		return functionArguments[0] - functionArguments[1];
	} 
	else if (functionName == RpnFunctionMultiply) {
		return functionArguments[0] * functionArguments[1];
	} 
	else if (functionName == RpnFunctionDivide) {
		return functionArguments[0] / functionArguments[1];
	} 
	else if (functionName == RpnFunctionPower) {
		return qPow(functionArguments[0],  functionArguments[1]);
	} 
	else if (functionName == Sine) {
		return qSin(functionArguments[0]);
	}
	else if (functionName == Cosine) {
		return qCos(functionArguments[0]);
	}
	else if (functionName == Tangent) {
		return qTan(functionArguments[0]);
	} else {
		throw Exception(tr("Unknown default function"));
	}
	
	// Compiler needs this useless thing
	return 0;
}

void ExprCalculator::addConstant(const QString &name, const Number &value)
{
	if (m_builtInConstants.contains(name)) {
		throw Exception(tr("Constant exists as built in"));
	}

	m_constants.insert(name, value);
}

void ExprCalculator::addFunction(const QString &name, const RpnFunction &function)
{
	if (m_builtInFunctions.contains(name)) {
		throw Exception(tr("Function exists as built in"));
	}

	m_functions.insert(name, function);
}

bool ExprCalculator::isFunction(const QString &name)
{
	return (m_functions.contains(name) || m_builtInFunctions.contains(name));
}

bool ExprCalculator::isConstant(const QString &name)
{
	return (m_constants.contains(name) || m_builtInConstants.contains(name));
}

int ExprCalculator::functionArgumentsCount(const QString &name)
{
	if (!isFunction(name)) {
		throw Exception(tr("There is no such a function"));
	}

	if (m_functions.contains(name)) {
		return m_functions.value(name).argumentsCount;
	}
	else {
		return m_builtInFunctions.value(name);
	}
}

void ExprCalculator::initializeBuiltInFunctions()
{
	m_builtInFunctions.insert(RpnFunctionMain, 0);
	m_builtInFunctions.insert(RpnFunctionPlus, 2);
	m_builtInFunctions.insert(RpnFunctionMinus, 2);
	m_builtInFunctions.insert(RpnFunctionMultiply, 2);
	m_builtInFunctions.insert(RpnFunctionDivide, 2);
	m_builtInFunctions.insert(RpnFunctionPower, 2);
	m_builtInFunctions.insert(Sine, 1);
	m_builtInFunctions.insert(Cosine, 1);
	m_builtInFunctions.insert(Tangent, 1);
}

void ExprCalculator::initializeBuiltInConstants()
{
	m_builtInConstants.insert(Pi, M_PI);
	m_builtInConstants.insert(E, M_E);
}
