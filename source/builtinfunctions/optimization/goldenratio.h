#ifndef GOLDENRATIO_H
#define GOLDENRATIO_H

#include "../builtinfunction.h"

// Class for finding minimum of function with Golden Ratio method
class GoldenRatio : public BuiltInFunction
{
public:
	GoldenRatio() : BuiltInFunction("golden_ratio")
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

#endif // GOLDENRATIO_H
