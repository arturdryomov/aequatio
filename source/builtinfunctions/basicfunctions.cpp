#include "basicfunctions.h"

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
	Sine instanceCalcSine;
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
	Cosine instanceCalcCosine;
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
	Tangent instanceCalcTangent;
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
	Abs instanceAbs;
}

RpnOperand Abs::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qAbs(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<RpnArgument> Abs::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	Sqrt instanceSqrt;
}

RpnOperand Sqrt::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qSqrt(actualArguments.at(0).value.value<Number>());

	return result;
}

QList<RpnArgument> Sqrt::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}
