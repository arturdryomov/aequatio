#include "integral.h"

namespace BuiltIn {
namespace BasicFunctions {

namespace
{
	Integral instance;
}

Integral::Integral() :
	Function("integral")
{
}

Rpn::Operand Integral::calculate(Function::FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_left = actualArguments.at(1).value.value<Number>();
	m_right = actualArguments.at(2).value.value<Number>();
	m_epsilon = actualArguments.at(3).value.value<Number>();

	try {
		Number result = findSolution();
		Rpn::Operand operand(Rpn::OperandNumber, QVariant::fromValue(result));
		return operand;
	}
	catch (ENoSolution) {
		return Rpn::Operand(Rpn::OperandIncorrect);
	}
}

QList<Rpn::Argument> Integral::requiredArguments()
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

Rpn::OperandType Integral::returnValueType()
{
	return Rpn::OperandNumber;
}

Number Integral::findSolution()
{
	if (m_left >= m_right) {
		throw EWrongArgument("Right limit", "greater than left limit");
	}

	int stepsCount = 4;
	Number result = integrateWithStepsCount(stepsCount);
	stepsCount *= 2;

	forever {
		Number newResult = integrateWithStepsCount(stepsCount);
		Number calculationError = qAbs(newResult - result) / 3.0;

		if (calculationError < m_epsilon) {
			return newResult;
		}
		else {
			result = newResult;
			stepsCount *= 2;
		}
	}
}

Number Integral::integrateWithStepsCount(int stepsCount)
{
	Number step = (m_right - m_left) / stepsCount;

	Number intervalLeft = m_left;
	Number result = 0.0;
	for (int i = 0; i < stepsCount; ++i) {
		result += findAreaOnInterval(intervalLeft, intervalLeft + step);
		intervalLeft += step;
	}

	if (!MathUtils::isFinite(result)) {
		throw ENoSolution();
	}
	return result;
}

Number Integral::findAreaOnInterval(Number left, Number right)
{
	Number middle = left + (right - left) / 2;
	Number functionLeft = calculateFunction(left);
	Number functionMiddle = calculateFunction(middle);
	Number functionRight = calculateFunction(right);
	Number step = right - left;

	return (step / 6.0) * (functionLeft + 4 * functionMiddle + functionRight);
}

Number Integral::calculateFunction(Number argument)
{
	QList<Rpn::Operand> functionArguments;
	Rpn::Operand functionArgument(Rpn::OperandNumber, argument);
	functionArguments << functionArgument;

	Rpn::Operand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

} // namespace
} // namespace
