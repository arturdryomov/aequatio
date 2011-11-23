#include "diffrungekutta.h"

namespace BuiltIn {
namespace BasicFunctions {

namespace
{
	DiffRungeKutta instance;
}

DiffRungeKutta::DiffRungeKutta() :
	Function("diff_runge_kutta")
{
}

Rpn::Operand DiffRungeKutta::calculate(Function::FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_initialValue = actualArguments.at(1).value.value<Number>();

	m_left = actualArguments.at(2).value.value<Number>();
	m_right = actualArguments.at(3).value.value<Number>();
	if (m_left >= m_right) {
		throw EWrongArgument("Right interval boundary", "greater that left one");
	}

	Number intervalsCount = actualArguments.at(4).value.value<Number>();
	if (!MathUtils::isInteger(intervalsCount)) {
		throw EWrongArgument("Number of intervals", "an integer value");
	}
	m_intervalsCount = intervalsCount;



	Rpn::Operand result;
	result.type = Rpn::OperandVector;
	result.value = QVariant::fromValue(Rpn::Vector::fromTwoDimensional(findSolution()));
	return result;
}

QList<Rpn::Argument> DiffRungeKutta::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
		// 1 is argument count in function that is passed as and argument
		<< Rpn::Argument(Rpn::OperandFunctionName, QString(), QVariant::fromValue(2))
		<< Rpn::Argument(Rpn::OperandNumber) // y0, initial value
		<< Rpn::Argument(Rpn::OperandNumber) // x0, left boundary of the interval
		<< Rpn::Argument(Rpn::OperandNumber) // xn, right boundary of the interval
		<< Rpn::Argument(Rpn::OperandNumber); // n, number of intervals

	return arguments;
}

Rpn::OperandType DiffRungeKutta::returnValueType()
{
	return Rpn::OperandVector;
}

QList<QList<Number> > DiffRungeKutta::findSolution()
{
	Number intervalSize = (m_right - m_left) / m_intervalsCount;

	QList<Number> arguments;
	QList<Number> values;


	// Calculate points

	Number previousValue = m_initialValue;
	Number previousArgument = m_left;

	for (int i = 0; i < m_intervalsCount; ++i) {
		Number k1 = intervalSize * calculateFunction(previousArgument, previousValue);
		Number k2 = intervalSize * calculateFunction(previousArgument + 0.5*intervalSize,
			previousValue + 0.5*k1);
		Number k3 = intervalSize * calculateFunction(previousArgument + 0.5*intervalSize,
			previousValue + 0.5*k2);
		Number k4 = intervalSize * calculateFunction(previousArgument + intervalSize,
			previousValue + k3);

		Number value = previousValue + 1.0/6.0 * (k1 + 2*k2 + 2*k3 + k4);
		Number argument = previousArgument + intervalSize;

		values << value;
		arguments << argument;

		previousValue = value;
		previousArgument = argument;
	}


	// Pack points to a list

	QList<Number> point;
	point << 0.0 << 0.0; // Reserve place for two items

	QList<QList<Number> > result;

	for (int i = 0; i < values.size(); ++i) {
		point[0] = arguments.at(i);
		point[1] = values.at(i);
		result << point;
	}

	return result;
}

Number DiffRungeKutta::calculateFunction(Number argument1, Number argument2)
{
	QList<Rpn::Operand> functionArguments;
	Rpn::Operand functionArgument1(Rpn::OperandNumber, argument1);
	Rpn::Operand functionArgument2(Rpn::OperandNumber, argument2);
	functionArguments << functionArgument1 << functionArgument2;

	Rpn::Operand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

} // namespace
} // namespace
