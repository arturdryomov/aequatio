#include "exprcalculator.h"

#include <QStack>
#include <qmath.h>

ExprCalculator::ExprCalculator(QObject *parent) : QObject(parent)
{
	initializeStandardFunctions();
}

void ExprCalculator::initializeStandardFunctions()
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

Number ExprCalculator::calculate(const RpnCode &code)
{
	m_rpnCode = code;
	if (!m_rpnCode.contains(RpnFunctionMain)) {
		throw Exception(tr("There is no main function"));
	}

	return calculateFunction(RpnFunctionMain, QList<Number>());
}

Number ExprCalculator::calculateFunction(QString functionName, QList<Number> functionArguments)
{
	RpnCodeThread currentThread = m_rpnCode.value(functionName).codeThread;
	QStack<Number> calculationStack;

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

				else if (m_rpnCode.contains(calledFunctionName)) {
					QList<Number> actualArguments;
					for (int i = 0; i < m_rpnCode.value(calledFunctionName).argumentsCount; i++) {
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

bool ExprCalculator::isBuiltInFunction(const QString &functionName)
{
	return m_builtInFunctions.contains(functionName);
}

int ExprCalculator::builtInFunctionArgumentsCount(const QString &functionName)
{
	if (!isBuiltInFunction(functionName)) {
		throw Exception(tr("There is no such function"));
	}

	return m_builtInFunctions.value(functionName);
}
