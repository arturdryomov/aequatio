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
	QList<RpnArgument> requiredArguments();
};


class Minus : public BuiltInFunction
{
public:
	Minus() : BuiltInFunction(RpnFunctionMinus)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Multiply : public BuiltInFunction
{
public:
	Multiply() : BuiltInFunction(RpnFunctionMultiply)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Divide : public BuiltInFunction
{
public:
	Divide() : BuiltInFunction(RpnFunctionDivide)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Power : public BuiltInFunction
{
public:
	Power() : BuiltInFunction(RpnFunctionPower)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class UnaryMinus : public BuiltInFunction
{
public:
	UnaryMinus() : BuiltInFunction(RpnFunctionUnaryMinus)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class CalcSine : public BuiltInFunction
{
public:
	CalcSine() : BuiltInFunction("sin")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class CalcCosine : public BuiltInFunction
{
public:
	CalcCosine() : BuiltInFunction("cos")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class CalcTangent : public BuiltInFunction
{
public:
	CalcTangent() : BuiltInFunction("tan")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

class Abs : public BuiltInFunction
{
public:
	Abs() : BuiltInFunction("abs")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

class Sqrt : public BuiltInFunction
{
public:
	Sqrt() : BuiltInFunction("sqrt")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

#endif // BASICFUNCTIONS_H