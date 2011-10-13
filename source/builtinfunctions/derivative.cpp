#include "derivative.h"

namespace BuiltInFunctions {
namespace BasicFunctions {

namespace
{
	Derivative instance;
}

RpnOperand Derivative::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	Number point = actualArguments.at(1).value.value<Number>();

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = QVariant::fromValue(findSolution(point));
	return result;
}

QList<RpnArgument> Derivative::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		// 1 is argument count in function that is passed as and argument
		<< RpnArgument(RpnOperandFunctionName, QString(), QVariant::fromValue(1))
		<< RpnArgument(RpnOperandNumber);

	return arguments;
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
	QList<RpnOperand> functionArguments;
	RpnOperand functionArgument(RpnOperandNumber, argument);
	functionArguments << functionArgument;

	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
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
