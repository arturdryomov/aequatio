#ifndef RETURNING_RANDOM_H
#define RETURNING_RANDOM_H

#include "../function.h"

namespace BuiltIn {
namespace Optimization {

// Class for finding minimum of function with Returning Random method
class ReturningRandom : public Function
{
public:
	ReturningRandom();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();

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

} // namespace
} // namespace

#endif // RETURNING_RANDOM_H
