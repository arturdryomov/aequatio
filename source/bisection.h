#ifndef BISECTION_H
#define BISECTION_H

#include "builtinfunction.h"

// Class for finding minimum of function with Bisection method
class Bisection : public BuiltInFunction
{
public:
	Bisection() : BuiltInFunction("bisection")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	Number countFunction(Number argument);
	Number findMinimum();

	FunctionCalculator* m_calculator;
	QString m_functionName;
	Number m_accuracy;

	struct Interval {
		Number leftBorder;
		Number rightBorder;
	};
	Interval m_sourceInterval;
};

#endif // BISECTION_H
