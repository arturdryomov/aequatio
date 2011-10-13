#ifndef DERIVATIVE_H
#define DERIVATIVE_H

#include "builtinfunction.h"

namespace BuiltInFunctions {
namespace BasicFunctions {

class Derivative : public Function
{
public:
	Derivative() : Function("derivative")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	FunctionCalculator* m_calculator;
	QString m_functionName;

	Number findSolution(Number point);
	Number countFunction(Number argument);
};

} // namespace
} // namespace

#endif // DERIVATIVE_H
