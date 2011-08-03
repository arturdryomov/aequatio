#ifndef BASICFUNCTIONS_H
#define BASICFUNCTIONS_H

#include "builtinfunction.h"

class Plus : public BuiltInFunction
{
public:
	Plus() : BuiltInFunction(RpnFunctionPlus)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
};


class Minus : public BuiltInFunction
{
public:
	Minus() : BuiltInFunction(RpnFunctionMinus)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
};


class Multiply : public BuiltInFunction
{
public:
	Multiply() : BuiltInFunction(RpnFunctionMultiply)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
};


class Divide : public BuiltInFunction
{
public:
	Divide() : BuiltInFunction(RpnFunctionDivide)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
};


class Power : public BuiltInFunction
{
public:
	Power() : BuiltInFunction(RpnFunctionPower)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
};


class UnaryMinus : public BuiltInFunction
{
public:
	UnaryMinus() : BuiltInFunction(RpnFunctionUnaryMinus)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
};


class CalcSine : public BuiltInFunction
{
public:
	CalcSine() : BuiltInFunction(Sine)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
};


class CalcCosine : public BuiltInFunction
{
public:
	CalcCosine() : BuiltInFunction(Cosine)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
};


class CalcTangent : public BuiltInFunction
{
public:
	CalcTangent() : BuiltInFunction(Tangent)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
};

#endif // BASICFUNCTIONS_H
