#ifndef DICHOTOMY_H
#define DICHOTOMY_H

#include "../function.h"

namespace BuiltIn {
namespace Optimization {

// Class for finding minimum of function with Dichotomy method
class Dichotomy : public Function
{
public:
	Dichotomy() : Function("dichotomy")
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
	Number m_swing;
	Number m_accuracy;

	Number findMinimum();
	Number countFunction(Number argument);
};

} // namespace
} // namespace

#endif // DICHOTOMY_H
