#include "enumerative.h"

namespace BuiltInFunctions {
namespace Optimization {

namespace
{
	Enumerative instance;
}

RpnOperand Enumerative::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_sourceInterval.leftBorder = actualArguments.at(1).value.value<Number>();
	m_sourceInterval.rightBorder = actualArguments.at(2).value.value<Number>();
	m_iterationsCount = actualArguments.at(3).value.value<Number>();

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = QVariant::fromValue(findMinimum());
	return result;
}

QList<RpnArgument> Enumerative::requiredArguments()
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

Number Enumerative::findMinimum()
{
	Number minimumPoint = m_sourceInterval.leftBorder +
		(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder) /
		(m_iterationsCount + 1);
	Number functionMinumum = countFunction(minimumPoint);

	for (int i = 1; i <= m_iterationsCount; i++) {
		Number currentPoint = m_sourceInterval.leftBorder + i *
			(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder) /
			(m_iterationsCount + 1);

		if (countFunction(currentPoint) < functionMinumum) {
			functionMinumum = countFunction(currentPoint);
			minimumPoint = currentPoint;
		}
	}

	return minimumPoint;
}

Number Enumerative::countFunction(Number argument)
{
	QList<RpnOperand> functionArguments;
	RpnOperand functionArgument(RpnOperandNumber, argument);
	functionArguments << functionArgument;

	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

} // namespace
} // namespace
