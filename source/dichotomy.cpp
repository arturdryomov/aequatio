#include "dichotomy.h"

#include <qmath.h>

namespace
{
	Dichotomy instance;
}

RpnOperand Dichotomy::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;
	m_functionName = actualArguments[0].value.value<QString>();
	m_sourceInterval.leftBorder = actualArguments[1].value.value<Number>();
	m_sourceInterval.rightBorder = actualArguments[2].value.value<Number>();
	m_accuracy = actualArguments[3].value.value<Number>();
	m_space = actualArguments[4].value.value<Number>();

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = findMinimum();
	return result;
}

QList<RpnArgument> Dichotomy::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		// 1 is argument count in function that is passed as and argument
		<< RpnArgument(RpnOperandFunctionName, QString(), QVariant::fromValue(1))
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber);

	return arguments;
}

Number Dichotomy::findMinimum()
{
	forever {
		Interval newInterval;
		newInterval.leftBorder = (m_sourceInterval.leftBorder +
			m_sourceInterval.rightBorder - m_space) / 2;
		newInterval.rightBorder = (m_sourceInterval.leftBorder +
			m_sourceInterval.rightBorder + m_space) / 2;

		Number leftFunctionValue = countFunction(newInterval.leftBorder);
		Number rightFunctionValue = countFunction(newInterval.rightBorder);
		if (leftFunctionValue <= rightFunctionValue) {
			m_sourceInterval.rightBorder = newInterval.rightBorder;
		}
		else {
			m_sourceInterval.leftBorder = newInterval.leftBorder;
		}

		if (qAbs(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder) <= m_accuracy) {
			// Exit condition
			return (m_sourceInterval.leftBorder + m_sourceInterval.rightBorder) / 2;
		}
	}
}

Number Dichotomy::countFunction(Number argument)
{
	QList<RpnOperand> functionArguments;
	RpnOperand functionArgument(RpnOperandNumber, argument);
	functionArguments << functionArgument;

	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}
