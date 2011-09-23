#ifndef BASICFUNCTIONS_H
#define BASICFUNCTIONS_H

#include "function.h"

namespace BuiltIn {
namespace BasicFunctions{

class Plus : public Function
{
public:
	Plus() : Function(Rpn::FunctionPlus)
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class Minus : public Function
{
public:
	Minus() : Function(Rpn::FunctionMinus)
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class Multiply : public Function
{
public:
	Multiply() : Function(Rpn::FunctionMultiply)
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class Divide : public Function
{
public:
	Divide() : Function(Rpn::FunctionDivide)
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class Power : public Function
{
public:
	Power() : Function(Rpn::FunctionPower)
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class UnaryMinus : public Function
{
public:
	UnaryMinus() : Function(Rpn::FunctionUnaryMinus)
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class Sine : public Function
{
public:
	Sine() : Function("sin")
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class Cosine : public Function
{
public:
	Cosine() : Function("cos")
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class Tangent : public Function
{
public:
	Tangent() : Function("tan")
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class Arcsine : public Function
{
public:
	Arcsine() : Function("asin")
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class Arccosine : public Function
{
public:
	Arccosine() : Function("acos")
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class Arctangent : public Function
{
public:
	Arctangent() : Function("atan")
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class AbsoluteValue : public Function
{
public:
	AbsoluteValue() : Function("abs")
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class SquareRoot : public Function
{
public:
	SquareRoot() : Function("sqrt")
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class NaturalLogarithm : public Function
{
public:
	NaturalLogarithm() : Function("ln")
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};


class ExponentialFunction : public Function
{
public:
	ExponentialFunction() : Function("exp")
	{
	}

    Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
};

} // namespace
} // namespace

#endif // BASICFUNCTIONS_H
