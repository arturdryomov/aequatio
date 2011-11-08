#ifndef DIFFRUNGEKUTTA_H
#define DIFFRUNGEKUTTA_H

#include "function.h"

namespace BuiltIn {
namespace BasicFunctions {

class DiffRungeKutta : public Function
{
public:
	DiffRungeKutta();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();

private:
	FunctionCalculator* m_calculator;
	QString m_functionName;
	Number m_initialValue;
	Number m_left;
	Number m_right;
	int m_intervalsCount;

	QList<QList<Number> > findSolution();
	Number calculateFunction(Number argument1, Number argument2);
};

} // namespace
} // namespace

#endif // DIFFRUNGEKUTTA_H
