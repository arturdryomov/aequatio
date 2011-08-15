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
	struct Interval {
		Number leftBorder;
		Number rightBorder;
	};
	Interval m_sourceInterval;

	FunctionCalculator* m_calculator;
	QString m_functionName;
	Number m_iterationNumber;

	Number countFunction(Number argument);

	Number findMinimum();
};

#endif // UNIFORM_H
