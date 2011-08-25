#ifndef FIBONACCI_H
#define FIBONACCI_H

#include "../builtinfunction.h"

// Class for finding minimum of function with Fibonacci method
class Fibonacci : public BuiltInFunction
{
public:
	Fibonacci() : BuiltInFunction("fibonacci")
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
	Number m_difference;
	Number m_iterationsNumber;

	Number findMinimum();
	void initializeIterationsNumber();
	Number fibonacciNumber(int position);
	Number countFunction(Number argument);
};

#endif // FIBONACCI_H
