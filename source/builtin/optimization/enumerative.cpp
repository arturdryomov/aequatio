#include "enumerative.h"

namespace BuiltIn {
namespace Optimization {

namespace
{
	Enumerative instance;
}

Rpn::Operand Enumerative::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_sourceInterval.leftBorder = actualArguments.at(1).value.value<Number>();
	m_sourceInterval.rightBorder = actualArguments.at(2).value.value<Number>();
	m_iterationsCount = actualArguments.at(3).value.value<Number>();

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = QVariant::fromValue(findMinimum());
	return result;
}

QList<Rpn::Argument> Enumerative::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
		// 1 is argument count in function that is passed as and argument
		<< Rpn::Argument(Rpn::OperandFunctionName, QString(), QVariant::fromValue(1))
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}

Rpn::OperandType Enumerative::returnValueType()
{
	return Rpn::OperandNumber;
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
	QList<Rpn::Operand> functionArguments;
	Rpn::Operand functionArgument(Rpn::OperandNumber, argument);
	functionArguments << functionArgument;

	Rpn::Operand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

} // namespace
} // namespace
