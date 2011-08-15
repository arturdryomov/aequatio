#include "bisection.h"

// TODO: Maybe better call it from higher level
#include <qmath.h>

namespace
{
	Bisection instance;
}

RpnOperand Bisection::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;
	m_functionName = actualArguments[0].value.value<QString>();
	m_sourceInterval.leftBorder = actualArguments[1].value.value<Number>();
	m_sourceInterval.rightBorder = actualArguments[2].value.value<Number>();
	m_accuracy = actualArguments[3].value.value<Number>();

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = findMinimum();
	return result;
}

QList<RpnArgument> Bisection::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		// 1 is argument count in function that is passed as and argument
		<< RpnArgument(RpnOperandFunctionName, QString(), QVariant::fromValue(1))
		// TODO: Sometime change to vector interval argument (and for other classes too)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber);

	return arguments;
}

Number Bisection::findMinimum()
{
	Number middlePoint = (m_sourceInterval.leftBorder + m_sourceInterval.rightBorder) / 2;

	forever {
		Interval newInterval = {
			m_sourceInterval.leftBorder +
				qAbs(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder) / 4,
			m_sourceInterval.rightBorder -
				qAbs(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder) / 4
		};

		if (countFunction(newInterval.leftBorder) < countFunction(middlePoint)) {
			m_sourceInterval.rightBorder = middlePoint;
			middlePoint = newInterval.leftBorder;
		}
		else {
			if (countFunction(newInterval.rightBorder) < countFunction(middlePoint)) {
				m_sourceInterval.leftBorder = middlePoint;
				middlePoint = newInterval.rightBorder;
			}
			else {
				m_sourceInterval.leftBorder = newInterval.leftBorder;
				m_sourceInterval.rightBorder = newInterval.rightBorder;
			}
		}

		if (qAbs(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder) <= m_accuracy) {
			// Exit condition
			return middlePoint;
		}
	}
}

Number Bisection::countFunction(Number argument)
{
	QList<RpnOperand> functionArguments;
	RpnOperand functionArgument(RpnOperandNumber, argument);
	functionArguments << functionArgument;

	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}
