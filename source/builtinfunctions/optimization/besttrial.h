#ifndef BEST_TRIAL_H
#define BEST_TRIAL_H

#include "../builtinfunction.h"

// Class for finding minimum of function with Best Trial method
class BestTrial : public BuiltInFunction
{
public:
	BestTrial() : BuiltInFunction("best_trial")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	FunctionCalculator* m_calculator;
	QString m_functionName;
	QList<Number> m_sourcePoint;
	Number m_decrease;
	Number m_stepsCount;
	Number m_iterationsCount;
	Number m_minimumStepSize;
	Number m_stepSize;

	QList<Number> findMinimum();
	QList<Number> getSpecialMinimum(QList<QList<Number> > points);

	Number countFunction(QList<Number> arguments);
};

#endif // BEST_TRIAL_H