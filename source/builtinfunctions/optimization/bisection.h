#ifndef BISECTION_H
#define BISECTION_H

#include "../builtinfunction.h"

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
	struct Interval {
		Number leftBorder;
		Number rightBorder;
	};

	FunctionCalculator* m_calculator;
	QString m_functionName;
	Number m_accuracy;
	Interval m_sourceInterval;

	Number findMinimum();
	Number countFunction(Number argument);
};

#endif // BISECTION_H
