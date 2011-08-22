#ifndef ADAPTIVE_RANDOM_H
#define ADAPTIVE_RANDOM_H

#include "../builtinfunction.h"

// Class for finding minimum of function with Adaptive Random method
class AdaptiveRandom : public BuiltInFunction
{
public:
	AdaptiveRandom() : BuiltInFunction("adaptive_random")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	FunctionCalculator* m_calculator;
	QString m_functionName;
	QList<Number> m_sourcePoint;
	Number m_acceleration;
	Number m_decrease;
	Number m_wrongStepsCount;
	Number m_iterationsCount;
	Number m_minimumStepSize;
	Number m_stepSize;

	QList<Number> findMinimum();

	Number countFunction(QList<Number> arguments);

	QList<Number> generateRandomNumbers(int count, Number lowerLimit, Number higherLimit);
	Number getRandomNumber(Number limit);
};

#endif // ADAPTIVE_RANDOM_H
