#ifndef UNIFORM_H
#define UNIFORM_H

#include "builtinfunction.h"

// Class for finding minimum of function with Unirorm method
class Unirorm : public BuiltInFunction
{
public:
	Unirorm() : BuiltInFunction("uniform")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	Number countFunction(Number argument);
	Number findMinimum();

	FunctionCalculator* m_calculator;
	QString m_functionName;
	Number m_iterationNumber;

	struct Interval {
		Number leftBorder;
		Number rightBorder;
	};
	Interval m_sourceInterval;
};

#endif // UNIFORM_H
