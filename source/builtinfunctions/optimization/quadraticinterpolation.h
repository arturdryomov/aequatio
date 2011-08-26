#ifndef QUADRATIC_INTERPOLATION_H
#define QUADRATIC_INTERPOLATION_H

#include "../builtinfunction.h"

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

#endif // QUADRATIC_INTERPOLATION_H
