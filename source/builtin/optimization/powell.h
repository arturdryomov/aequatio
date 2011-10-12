#ifndef POWELL_H
#define POWELL_H

#include "../function.h"

namespace BuiltIn {
namespace Optimization {

// Class for finding minimum of function with Powell method
class Powell : public Function
{
public:
	Powell() : Function("powell") {}

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
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

} // namespace
} // namespace

#endif // POWELL_H
