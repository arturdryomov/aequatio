#include "derivative.h"

namespace BuiltIn {
namespace BasicFunctions {

namespace
{
	Derivative instance;
}

Derivative::Derivative() : Function("derivative")
{
}

Rpn::Operand Derivative::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	Number point = actualArguments.at(1).value.value<Number>();

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = QVariant::fromValue(findSolution(point));
	return result;
}

QList<Rpn::Argument> Derivative::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
		// 1 is argument count in function that is passed as and argument
		<< Rpn::Argument(Rpn::OperandFunctionName, QString(), QVariant::fromValue(1))
		<< Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}

Rpn::OperandType Derivative::returnValueType()
{
	return Rpn::OperandNumber;
}

Number Derivative::findSolution(Number point)
{
	Number result = 0;

	result -= countFunction(point + 2 * smallNumber(point));
	result += 8 * countFunction(point + smallNumber(point));
	result -= 8 * countFunction(point - smallNumber(point));
	result += countFunction(point - 2 * smallNumber(point));
	result /= 12 * smallNumber(point);

	return result;
}

Number Derivative::countFunction(Number argument)
{
	QList<Rpn::Operand> functionArguments;
	Rpn::Operand functionArgument(Rpn::OperandNumber, argument);
	functionArguments << functionArgument;

	Rpn::Operand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

Number Derivative::smallNumber(Number x)
{
	if (!MathUtils::isNull(x)) {
		return qSqrt(MathUtils::machineEpsilon()) * x;
	}
	else {
		return MathUtils::machineEpsilon();
	}
}

} // namespace
} // namespace
