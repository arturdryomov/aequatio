#include "basicfunctions.h"

namespace BuiltIn {
namespace BasicFunctions{

namespace
{
	Plus instancePlus;
}

Rpn::Operand Plus::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = actualArguments.at(0).value.value<Number>() + actualArguments.at(1).value.value<Number>();

	return result;
}

QList<Rpn::Argument> Plus::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber) << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	Minus instanceMinus;
}

Rpn::Operand Minus::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = actualArguments.at(0).value.value<Number>() - actualArguments.at(1).value.value<Number>();

	return result;
}

QList<Rpn::Argument> Minus::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber) << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	Multiply instanceMultiply;
}

Rpn::Operand Multiply::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = actualArguments.at(0).value.value<Number>() * actualArguments.at(1).value.value<Number>();

	return result;
}

QList<Rpn::Argument> Multiply::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber) << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	Divide instanceDivide;
}

Rpn::Operand Divide::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = actualArguments.at(0).value.value<Number>() / actualArguments.at(1).value.value<Number>();

	return result;
}

QList<Rpn::Argument> Divide::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber) << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	Power instancePower;
}

Rpn::Operand Power::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = qPow(actualArguments.at(0).value.value<Number>(), actualArguments.at(1).value.value<Number>());

	return result;
}

QList<Rpn::Argument> Power::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber) << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	UnaryMinus instanceUnaryMinus;
}

Rpn::Operand UnaryMinus::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = -actualArguments.at(0).value.value<Number>();

	return result;
}

QList<Rpn::Argument> UnaryMinus::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	Sine instanceSine;
}

Rpn::Operand Sine::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = qSin(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<Rpn::Argument> Sine::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	Cosine instanceCosine;
}

Rpn::Operand Cosine::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = qCos(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<Rpn::Argument> Cosine::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	Tangent instanceTangent;
}

Rpn::Operand Tangent::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = qTan(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<Rpn::Argument> Tangent::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	Arcsine instanceArcsine;
}

Rpn::Operand Arcsine::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = qAsin(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<Rpn::Argument> Arcsine::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	Arccosine instanceArccosine;
}

Rpn::Operand Arccosine::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = qAcos(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<Rpn::Argument> Arccosine::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	Arctangent instanceArctangent;
}

Rpn::Operand Arctangent::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = qAtan(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<Rpn::Argument> Arctangent::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	AbsoluteValue instanceAbsoluteValue;
}

Rpn::Operand AbsoluteValue::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = qAbs(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<Rpn::Argument> AbsoluteValue::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	SquareRoot instanceSquareRoot;
}

Rpn::Operand SquareRoot::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = qSqrt(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<Rpn::Argument> SquareRoot::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	NaturalLogarithm instanceNaturalLogarithm;
}

Rpn::Operand NaturalLogarithm::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = qLn(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<Rpn::Argument> NaturalLogarithm::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}


namespace
{
	ExponentialFunction instanceExponentialFunction;
}

Rpn::Operand ExponentialFunction::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = qExp(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<Rpn::Argument> ExponentialFunction::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}

} // namespace
} // namespace
