#ifndef POWELL_H
#define POWELL_H

#include "../builtinfunction.h"

// Class for finding minimum of function with Powell method
class Powell : public BuiltInFunction
{
public:
	Powell() : BuiltInFunction("powell")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	FunctionCalculator* m_calculator;
	QString m_functionName;
	Number m_sourcePoint;
	Number m_stepSize;
	Number m_firstAccuracyCoefficient;
	Number m_secondAccuracyCoefficient;

	Number findMinimum();
	Number getMinimumPoint(Number first, Number second, Number third);
	Number countFunction(Number argument);
};

#endif // POWELL_H
