#include "uniform.h"

namespace
{
	Uniform instance;
}

RpnOperand Uniform::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;
	m_functionName = actualArguments[0].value.value<QString>();
	m_sourceInterval.leftBorder = actualArguments[1].value.value<Number>();
	m_sourceInterval.rightBorder = actualArguments[2].value.value<Number>();
	m_iterationNumber = actualArguments[3].value.value<Number>();

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = findMinimum();
	return result;
}

QList<RpnArgument> Uniform::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		// 1 is argument count in function that is passed as and argument
		<< RpnArgument(RpnOperandFunctionName, QString(), QVariant::fromValue(1))
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber);

	return arguments;
}

Number Uniform::findMinimum()
{
	Number pointMinimum = m_sourceInterval.leftBorder +
		(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder) /
		(m_iterationNumber + 1);
	Number functionMinumum = countFunction(pointMinimum);

	for (int i = 1; i <= m_iterationNumber; i++) {
		Number point = m_sourceInterval.leftBorder + i *
			(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder) /
			(m_iterationNumber + 1);

		if (countFunction(point) < functionMinumum) {
			functionMinumum = countFunction(point);
			pointMinimum = point;
		}
	}

	return pointMinimum;
}

Number Uniform::countFunction(Number argument)
{
	QList<RpnOperand> functionArguments;
	RpnOperand functionArgument(RpnOperandNumber, argument);
	functionArguments << functionArgument;

	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}
