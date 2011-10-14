#ifndef BISECTION_H
#define BISECTION_H

#include "../function.h"

namespace BuiltIn {
namespace Optimization {

// Class for finding minimum of function with Bisection method
class Bisection : public Function
{
public:
	Bisection();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();

private:
	struct Interval {
		Number leftBorder;
		Number rightBorder;
	};

	FunctionCalculator* m_calculator;
	QString m_functionName;
	Number m_accuracy;
	Interval m_sourceInterval;

	Number findMinimum();
	Number countFunction(Number argument);
};

} // namespace
} // namespace

#endif // BISECTION_H
