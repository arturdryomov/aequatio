#ifndef BASICFUNCTIONS_H
#define BASICFUNCTIONS_H

#include "builtinfunction.h"

namespace BuiltInFunctions {
namespace BasicFunctions{

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


class Sine : public BuiltInFunction
{
public:
	Sine() : BuiltInFunction("sin")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Cosine : public BuiltInFunction
{
public:
	Cosine() : BuiltInFunction("cos")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Tangent : public BuiltInFunction
{
public:
	Tangent() : BuiltInFunction("tan")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Arcsine : public BuiltInFunction
{
public:
	Arcsine() : BuiltInFunction("asin")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Arccosine : public BuiltInFunction
{
public:
	Arccosine() : BuiltInFunction("acos")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Arctangent : public BuiltInFunction
{
public:
	Arctangent() : BuiltInFunction("atan")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class AbsoluteValue : public BuiltInFunction
{
public:
	AbsoluteValue() : BuiltInFunction("abs")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class SquareRoot : public BuiltInFunction
{
public:
	SquareRoot() : BuiltInFunction("sqrt")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class NaturalLogarithm : public BuiltInFunction
{
public:
	NaturalLogarithm() : BuiltInFunction("ln")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class ExponentialFunction : public BuiltInFunction
{
public:
	ExponentialFunction() : BuiltInFunction("exp")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

} // namespace
} // namespace

#endif // BASICFUNCTIONS_H
