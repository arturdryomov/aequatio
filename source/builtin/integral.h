#ifndef INTEGRAL_H
#define INTEGRAL_H

#include "function.h"

namespace BuiltIn {
namespace BasicFunctions {

class Integral : public Function
{
public:
	Integral();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();

private:
	class ENoSolution : public ECalculating {};

	FunctionCalculator *m_calculator;
	QString m_functionName;
	Number m_left;
	Number m_right;
	Number m_epsilon;

	Number findSolution();
	Number integrateWithStepsCount(int stepsCount);
	Number findAreaOnInterval(Number left, Number right);
	Number calculateFunction(Number argument);
};

} // namespace
} // namespace

#endif // INTEGRAL_H
