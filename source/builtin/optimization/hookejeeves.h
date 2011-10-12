#ifndef HOOKEJEEVES_H
#define HOOKEJEEVES_H

#include "../function.h"

namespace BuiltIn {
namespace Optimization {

// Class for finding minimum of function with Hooke-Jeeves method
class HookeJeeves : public Function
{
public:
	HookeJeeves() : Function("hooke_jeeves") {}

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
private:
	FunctionCalculator* m_calculator;
	QString m_functionName;
	QList<Number> m_sourcePoint;
	Number m_stopValue;
	QList<Number> m_stepSizes;
	Number m_accelerationCoefficient;
	Number m_decreaseCoefficient;

	QList<Number> findMinimum();
	QList<Number> increaseDirection(QList<Number> point, int direction);
	QList<Number> decreaseDirection(QList<Number> point, int direction);
	Number countFunction(QList<Number> arguments);
};

} // namespace
} // namespace

#endif // HOOKEJEEVES_H
