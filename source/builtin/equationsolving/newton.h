#ifndef NEWTON_H
#define NEWTON_H

#include "../function.h"

namespace BuiltIn {
namespace EquationSolving {

class Newton : public Function
{
public:
	Newton();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();

private:
	struct Interval {
		Number leftBorder;
		Number rightBorder;
	};

	Interval m_searchInterval;
	FunctionCalculator* m_calculator;
	QString m_functionName;
	Number m_ratio;
	Number m_accuracy;

	Number findSolution();
	Number countFunction(Number argument);
	Number countDerivative(Number argument);
};

} // namespace
} // namespace

#endif // NEWTON_H
