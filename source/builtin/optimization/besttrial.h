#ifndef BEST_TRIAL_H
#define BEST_TRIAL_H

#include "../function.h"

namespace BuiltIn {
namespace Optimization {

// Class for finding minimum of function with Best Trial method
class BestTrial : public Function
{
public:
	BestTrial() : Function("best_trial")
	{
	}

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
private:
	FunctionCalculator* m_calculator;
	QString m_functionName;
	QList<Number> m_sourcePoint;
	Number m_decreaseCoefficient;
	Number m_stepsCount;
	Number m_maximumIterationsCount;
	Number m_minimumStepSize;
	Number m_stepSize;

	QList<Number> findMinimum();
	QList<Number> getSpecialMinimum(QList<QList<Number> > points);
	Number countFunction(QList<Number> arguments);
};

} // namespace
} // namespace

#endif // BEST_TRIAL_H
