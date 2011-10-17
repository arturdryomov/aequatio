#ifndef GOLDENSECTION_H
#define GOLDENSECTION_H

#include "../function.h"

namespace BuiltIn {
namespace Optimization {

// Class for finding minimum of function with Golden Section method
class GoldenSection : public Function
{
public:
	GoldenSection();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();

private:
	struct Interval {
		Number leftBorder;
		Number rightBorder;
	};

	Interval m_sourceInterval;
	FunctionCalculator* m_calculator;
	QString m_functionName;
	Number m_ratio;
	Number m_accuracy;

	Number findMinimum();
	Number countFunction(Number argument);
};

} // namespace
} // namespace

#endif // GOLDENSECTION_H
