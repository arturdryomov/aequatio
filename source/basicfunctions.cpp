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
