#ifndef DERIVATIVE_H
#define DERIVATIVE_H

#include "function.h"

namespace BuiltIn {
namespace BasicFunctions {

class Derivative : public Function
{
public:
	Derivative();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();

private:
	FunctionCalculator* m_calculator;
	QString m_functionName;

	Number findSolution(Number point);
	Number calculateFunction(Number argument);
	Number smallNumber(Number x);
};

} // namespace
} // namespace

#endif // DERIVATIVE_H
