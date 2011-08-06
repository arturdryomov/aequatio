#ifndef DICHOTOMY_H
#define DICHOTOMY_H

#include "builtinfunction.h"

// Class for finding minimum of function with Dichotomy method
class Dichotomy : public BuiltInFunction
{
public:
	Dichotomy() : BuiltInFunction("dichotomy")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	Number countFunction(Number argument);
	Number findMinimum();

	FunctionCalculator* m_calculator;
	QString m_functionName;
	Number m_space;
	Number m_accuracy;

	struct Interval {
		Number leftBorder;
		Number rightBorder;
	};
	Interval m_sourceInterval;
};

#endif // DICHOTOMY_H
