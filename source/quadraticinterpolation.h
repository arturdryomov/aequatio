#ifndef QUADRATIC_INTERPOLATION_H
#define QUADRATIC_INTERPOLATION_H

#include "builtinfunction.h"

// Class for finding minimum of function with Quadratic Interpolation method
class QuadraticInterpolation : public BuiltInFunction
{
public:
	QuadraticInterpolation() : BuiltInFunction("quadratic_interpolation")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	Number countFunction(Number argument);
	Number findMinimum();
	Number minimumPointValue(Number first, Number second, Number third);

	FunctionCalculator* m_calculator;
	QString m_functionName;
	Number m_startPoint;
	Number m_stepSize;
	Number m_firstAccuracy;
	Number m_secondAccuracy;
};

#endif // QUADRATIC_INTERPOLATION_H
