#ifndef GOLDENRATIO_H
#define GOLDENRATIO_H

#include "builtinfunction.h"

struct Interval {
	Number leftBorder;
	Number rightBorder;
};

// Class for finding minimum of function with Golden Ratio method
class GoldenRatio : public BuiltInFunction
{
public:
	GoldenRatio() : BuiltInFunction("golden_ratio")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnOperand> requiredArguments();
private:
	Number countFunction(Number argument);
	Number findMinimum();

	FunctionCalculator* m_calculator;
	QString m_functionName;
	Number m_ratio;
	Number m_accuracy;
  Interval m_sourceInterval;
};

#endif // GOLDENRATIO_H
