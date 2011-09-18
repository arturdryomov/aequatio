#ifndef BASICFUNCTIONS_H
#define BASICFUNCTIONS_H

#include "function.h"

namespace BuiltIn {
namespace BasicFunctions{

class Plus : public Function
{
public:
	Plus() : Function(RpnFunctionPlus)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Minus : public Function
{
public:
	Minus() : Function(RpnFunctionMinus)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Multiply : public Function
{
public:
	Multiply() : Function(RpnFunctionMultiply)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Divide : public Function
{
public:
	Divide() : Function(RpnFunctionDivide)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Power : public Function
{
public:
	Power() : Function(RpnFunctionPower)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class UnaryMinus : public Function
{
public:
	UnaryMinus() : Function(RpnFunctionUnaryMinus)
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Sine : public Function
{
public:
	Sine() : Function("sin")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Cosine : public Function
{
public:
	Cosine() : Function("cos")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Tangent : public Function
{
public:
	Tangent() : Function("tan")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Arcsine : public Function
{
public:
	Arcsine() : Function("asin")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Arccosine : public Function
{
public:
	Arccosine() : Function("acos")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class Arctangent : public Function
{
public:
	Arctangent() : Function("atan")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class AbsoluteValue : public Function
{
public:
	AbsoluteValue() : Function("abs")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class SquareRoot : public Function
{
public:
	SquareRoot() : Function("sqrt")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class NaturalLogarithm : public Function
{
public:
	NaturalLogarithm() : Function("ln")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};


class ExponentialFunction : public Function
{
public:
	ExponentialFunction() : Function("exp")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

} // namespace
} // namespace

#endif // BASICFUNCTIONS_H
