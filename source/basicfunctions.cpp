#include "basicfunctions.h"

namespace
{
	Plus instance;
}

RpnOperand Plus::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = actualArguments[0].value.value<Number>() + actualArguments[1].value.value<Number>();

	return result;
}
