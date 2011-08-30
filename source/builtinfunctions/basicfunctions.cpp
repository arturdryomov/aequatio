#include "basicfunctions.h"

namespace BuiltInFunctions {
namespace BasicFunctions{

namespace
{
	Plus instancePlus;
}

RpnOperand Plus::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = actualArguments.at(0).value.value<Number>() + actualArguments.at(1).value.value<Number>();

	return result;
}

QList<RpnArgument> Plus::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber) << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	Minus instanceMinus;
}

RpnOperand Minus::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = actualArguments.at(0).value.value<Number>() - actualArguments.at(1).value.value<Number>();

	return result;
}

QList<RpnArgument> Minus::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber) << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	Multiply instanceMultiply;
}

RpnOperand Multiply::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = actualArguments.at(0).value.value<Number>() * actualArguments.at(1).value.value<Number>();

	return result;
}

QList<RpnArgument> Multiply::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber) << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	Divide instanceDivide;
}

RpnOperand Divide::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = actualArguments.at(0).value.value<Number>() / actualArguments.at(1).value.value<Number>();

	return result;
}

QList<RpnArgument> Divide::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber) << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	Power instancePower;
}

RpnOperand Power::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qPow(actualArguments.at(0).value.value<Number>(), actualArguments.at(1).value.value<Number>());

	return result;
}

QList<RpnArgument> Power::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber) << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	UnaryMinus instanceUnaryMinus;
}

RpnOperand UnaryMinus::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = -actualArguments.at(0).value.value<Number>();

	return result;
}

QList<RpnArgument> UnaryMinus::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	Sine instanceSine;
}

RpnOperand Sine::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qSin(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<RpnArgument> Sine::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	Cosine instanceCosine;
}

RpnOperand Cosine::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qCos(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<RpnArgument> Cosine::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	Tangent instanceTangent;
}

RpnOperand Tangent::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qTan(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<RpnArgument> Tangent::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	Arcsine instanceArcsine;
}

RpnOperand Arcsine::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qAsin(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<RpnArgument> Arcsine::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	Arccosine instanceArccosine;
}

RpnOperand Arccosine::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qAcos(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<RpnArgument> Arccosine::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	Arctangent instanceArctangent;
}

RpnOperand Arctangent::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qAtan(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<RpnArgument> Arctangent::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	AbsoluteValue instanceAbsoluteValue;
}

RpnOperand AbsoluteValue::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qAbs(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<RpnArgument> AbsoluteValue::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	SquareRoot instanceSquareRoot;
}

RpnOperand SquareRoot::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qSqrt(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<RpnArgument> SquareRoot::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	NaturalLogarithm instanceNaturalLogarithm;
}

RpnOperand NaturalLogarithm::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qLn(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<RpnArgument> NaturalLogarithm::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	ExponentialFunction instanceExponentialFunction;
}

RpnOperand ExponentialFunction::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qExp(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<RpnArgument> ExponentialFunction::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}

} // namespace
} // namespace
