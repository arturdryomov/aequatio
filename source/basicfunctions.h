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
	QList<RpnOperand> requiredArguments();
};


class Minus : public BuiltInFunction
{
public:
	Minus() : BuiltInFunction(RpnFunctionMinus)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnOperand> requiredArguments();
};


class Multiply : public BuiltInFunction
{
public:
	Multiply() : BuiltInFunction(RpnFunctionMultiply)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnOperand> requiredArguments();
};


class Divide : public BuiltInFunction
{
public:
	Divide() : BuiltInFunction(RpnFunctionDivide)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnOperand> requiredArguments();
};


class Power : public BuiltInFunction
{
public:
	Power() : BuiltInFunction(RpnFunctionPower)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnOperand> requiredArguments();
};


class UnaryMinus : public BuiltInFunction
{
public:
	UnaryMinus() : BuiltInFunction(RpnFunctionUnaryMinus)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnOperand> requiredArguments();
};


class CalcSine : public BuiltInFunction
{
public:
	CalcSine() : BuiltInFunction(Sine)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnOperand> requiredArguments();
};


class CalcCosine : public BuiltInFunction
{
public:
	CalcCosine() : BuiltInFunction(Cosine)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnOperand> requiredArguments();
};


class CalcTangent : public BuiltInFunction
{
public:
	CalcTangent() : BuiltInFunction(Tangent)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnOperand> requiredArguments();
};

#endif // BASICFUNCTIONS_H
