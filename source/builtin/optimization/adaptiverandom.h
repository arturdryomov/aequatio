#ifndef ADAPTIVE_RANDOM_H
#define ADAPTIVE_RANDOM_H

#include "../function.h"

namespace BuiltIn {
namespace Optimization {

// Class for finding minimum of function with Adaptive Random method
class AdaptiveRandom : public Function
{
public:
	AdaptiveRandom() : Function("adaptive_random") {}

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
private:
	FunctionCalculator* m_calculator;
	QString m_functionName;
	QList<Number> m_sourcePoint;
	Number m_accelerationCoefficient;
	Number m_decreaseCoefficient;
	Number m_wrongStepsCount;
	Number m_maximumIterationsCount;
	Number m_minimumStepSize;
	Number m_stepSize;

	QList<Number> findMinimum();
	Number countFunction(QList<Number> arguments);
};

} // namespace
} // namespace

#endif // ADAPTIVE_RANDOM_H
