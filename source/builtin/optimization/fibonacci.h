#ifndef FIBONACCI_H
#define FIBONACCI_H

#include "../function.h"

namespace BuiltIn {
namespace Optimization {

// Class for finding minimum of function with Fibonacci method
class Fibonacci : public Function
{
public:
	Fibonacci();

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
	Number m_resultIntervalLength;
	Number m_differenceConstant;
	int m_iterationsNumber;

	Number findMinimum();
	void initializeIterationsNumber();
	Number fibonacciNumber(int position);
	Number calculateFunction(Number argument);
};

} // namespace
} // namespace

#endif // FIBONACCI_H
