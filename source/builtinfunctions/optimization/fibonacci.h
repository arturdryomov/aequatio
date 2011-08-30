#ifndef FIBONACCI_H
#define FIBONACCI_H

#include "../builtinfunction.h"

namespace BuiltInFunctions {
namespace Optimization {

// Class for finding minimum of function with Fibonacci method
class Fibonacci : public Function
{
public:
	Fibonacci() : Function("fibonacci")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
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
	Number m_iterationsNumber;

	Number findMinimum();
	void initializeIterationsNumber();
	Number fibonacciNumber(int position);
	Number countFunction(Number argument);
};

} // namespace
} // namespace

#endif // FIBONACCI_H
