#ifndef RETURNING_RANDOM_H
#define RETURNING_RANDOM_H

#include "../builtinfunction.h"

// Class for finding minimum of function with Returning Random method
class ReturningRandom : public BuiltInFunction
{
public:
	ReturningRandom() : BuiltInFunction("returning_random")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	FunctionCalculator* m_calculator;
	QString m_functionName;
	QList<Number> m_sourcePoint;
	Number m_decreaseCoefficient;
	Number m_wrongStepsCount;
	Number m_maximumIterationsCount;
	Number m_minimumStepSize;
	Number m_stepSize;

	QList<Number> findMinimum();
	Number countFunction(QList<Number> arguments);
};

#endif // RETURNING_RANDOM_H
