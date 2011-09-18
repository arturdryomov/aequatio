#ifndef ENUMERATIVE_H
#define ENUMERATIVE_H

#include "../function.h"

namespace BuiltIn {
namespace Optimization {

// Class for finding minimum of function with Enumerative method
class Enumerative : public Function
{
public:
	Enumerative() : Function("enumerative")
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
	Number m_iterationsCount;

	Number findMinimum();
	Number countFunction(Number argument);
};

} // namespace
} // namespace

#endif // ENUMERATIVE_H
