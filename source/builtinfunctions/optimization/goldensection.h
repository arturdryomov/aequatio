#ifndef GOLDENSECTION_H
#define GOLDENSECTION_H

#include "../builtinfunction.h"

namespace BuiltInFunctions {
namespace Optimization {

// Class for finding minimum of function with Golden Section method
class GoldenSection : public Function
{
public:
	GoldenSection() : Function("golden_section")
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
	Number m_ratio;
	Number m_accuracy;

	Number findMinimum();
	Number countFunction(Number argument);
};

} // namespace
} // namespace

#endif // GOLDENSECTION_H
