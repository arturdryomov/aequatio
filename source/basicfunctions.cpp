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

QList<RpnOperand> Plus::requiredArguments()
{
	QList<RpnOperand> arguments;
	arguments << RpnOperand(RpnOperandNumber, QVariant()) << RpnOperand(RpnOperandNumber, QVariant());

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

QList<RpnOperand> Minus::requiredArguments()
{
	QList<RpnOperand> arguments;
	arguments << RpnOperand(RpnOperandNumber, QVariant()) << RpnOperand(RpnOperandNumber, QVariant());

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

QList<RpnOperand> Multiply::requiredArguments()
{
	QList<RpnOperand> arguments;
	arguments << RpnOperand(RpnOperandNumber, QVariant()) << RpnOperand(RpnOperandNumber, QVariant());

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

QList<RpnOperand> Divide::requiredArguments()
{
	QList<RpnOperand> arguments;
	arguments << RpnOperand(RpnOperandNumber, QVariant()) << RpnOperand(RpnOperandNumber, QVariant());

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

QList<RpnOperand> Power::requiredArguments()
{
	QList<RpnOperand> arguments;
	arguments << RpnOperand(RpnOperandNumber, QVariant()) << RpnOperand(RpnOperandNumber, QVariant());

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

QList<RpnOperand> UnaryMinus::requiredArguments()
{
	QList<RpnOperand> arguments;
	arguments << RpnOperand(RpnOperandNumber, QVariant());

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

QList<RpnOperand> CalcSine::requiredArguments()
{
	QList<RpnOperand> arguments;
	arguments << RpnOperand(RpnOperandNumber, QVariant());

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

QList<RpnOperand> CalcCosine::requiredArguments()
{
	QList<RpnOperand> arguments;
	arguments << RpnOperand(RpnOperandNumber, QVariant());

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

QList<RpnOperand> CalcTangent::requiredArguments()
{
	QList<RpnOperand> arguments;
	arguments << RpnOperand(RpnOperandNumber, QVariant());

	return arguments;
}
