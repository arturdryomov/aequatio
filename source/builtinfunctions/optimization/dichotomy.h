#ifndef DICHOTOMY_H
#define DICHOTOMY_H

#include "../builtinfunction.h"

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
	struct Interval {
		Number leftBorder;
		Number rightBorder;
	};
	Interval m_sourceInterval;

	FunctionCalculator* m_calculator;
	QString m_functionName;
	Number m_space;
	Number m_accuracy;

	Number countFunction(Number argument);

	Number findMinimum();
};

#endif // DICHOTOMY_H