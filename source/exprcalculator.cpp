#include "exprcalculator.h"

#include <QStack>
#include <qmath.h>

ExprCalculator::ExprCalculator(QObject *parent) : QObject(parent)
{
	initializeStandardFunctions();
}

void ExprCalculator::initializeStandardFunctions()
{
	m_standardFunctions.insert(RpnFunctionMain, 0);
	m_standardFunctions.insert(RpnFunctionPlus, 2);
	m_standardFunctions.insert(RpnFunctionMinus, 2);
	m_standardFunctions.insert(RpnFunctionMultiply, 2);
	m_standardFunctions.insert(RpnFunctionDivide, 2);
	m_standardFunctions.insert(RpnFunctionPower, 2);
}

Number ExprCalculator::calculate(const RpnCode &code)
{
	m_rpnCode = code;
	if (!m_rpnCode.contains(RpnFunctionMain)) {
		throw Exception(tr("There is no main function"));
	}

	return calculateFunction(RpnFunctionMain, QVector<Number>());
}

Number ExprCalculator::calculateFunction(QString functionName, QVector<Number> functionArguments)
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

				if (m_standardFunctions.contains(calledFunctionName)) {
					QVector<Number> actualArguments;
					for (int i = 0; i < m_standardFunctions.value(calledFunctionName); i++) {
						actualArguments.append(calculationStack.pop());
					}
					Number result = calculateBuiltInFunction(calledFunctionName, actualArguments);
					calculationStack.push(result);
				}

				else if (m_rpnCode.contains(calledFunctionName)) {
					QVector<Number> actualArguments;
					for (int i = 0; i < m_rpnCode.value(calledFunctionName).argumentsCount; i++) {
						actualArguments.append(calculationStack.pop());
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

Number ExprCalculator::calculateBuiltInFunction(QString functionName, QVector<Number> functionArguments)
{
	if (functionName == RpnFunctionPlus) {
		return functionArguments[1] + functionArguments[0];
	} else if (RpnFunctionMinus == functionName) {
		return functionArguments[1] - functionArguments[0];
	}	else if (functionName == RpnFunctionMultiply) {
		return functionArguments[1] * functionArguments[0];
	} else if (RpnFunctionDivide == functionName) {
		return functionArguments[1] / functionArguments[0];
	} else if (functionName == RpnFunctionPower) {
		return qPow(functionArguments[1],  functionArguments[0]);
	} else {
		throw Exception(tr("Unknown default function"));
	}

	// Compiler needs this useless thing
	return 0;
}

bool ExprCalculator::isBuiltInFunction(const QString &functionName)
{
	return m_standardFunctions.contains(functionName);
}

int ExprCalculator::builtInFunctionArgumentsCount(const QString &functionName)
{
	if (!isBuiltInFunction(functionName)) {
		throw Exception(tr("There is no such function"));
	}

	return m_standardFunctions.value(functionName);
}
