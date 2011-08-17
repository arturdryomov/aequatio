#include "basicfunctions.h"

#include <qmath.h>

namespace
{
	Plus instancePlus;
}

RpnOperand Plus::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = actualArguments[0].value.value<Number>() + actualArguments[1].value.value<Number>();

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
	result.value = actualArguments[0].value.value<Number>() - actualArguments[1].value.value<Number>();

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
	result.value = actualArguments[0].value.value<Number>() * actualArguments[1].value.value<Number>();

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
	result.value = actualArguments[0].value.value<Number>() / actualArguments[1].value.value<Number>();

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
	result.value = qPow(actualArguments[0].value.value<Number>(), actualArguments[1].value.value<Number>());

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
	result.value = -actualArguments[0].value.value<Number>();

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
	CalcSine instanceCalcSine;
}

RpnOperand CalcSine::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qSin(actualArguments[0].value.value<Number>());

	return result;
}

QList<RpnArgument> CalcSine::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	CalcCosine instanceCalcCosine;
}

RpnOperand CalcCosine::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qCos(actualArguments[0].value.value<Number>());

	return result;
}

QList<RpnArgument> CalcCosine::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}


namespace
{
	CalcTangent instanceCalcTangent;
}

RpnOperand CalcTangent::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = qTan(actualArguments[0].value.value<Number>());

	return result;
}

QList<RpnArgument> CalcTangent::requiredArguments()
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
	result.value = qAbs(actualArguments[0].value.value<Number>());

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
	result.value = qSqrt(actualArguments[0].value.value<Number>());

	return result;
}

QList<RpnArgument> Sqrt::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandNumber);

	return arguments;
}
