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

#endif // BASICFUNCTIONS_H
