#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "builtinfunction.h"

// Class for finding minimum of function with Configuration method
class Configuration : public BuiltInFunction
{
public:
	Configuration() : BuiltInFunction("configuration")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	Number countFunction(QList<Number> arguments);
	QList<Number> findMinimum();
	QList<Number> increaseDirection(QList<Number> point, int direction);
	QList<Number> decreaseDirection(QList<Number> point, int direction);

	FunctionCalculator* m_calculator;
	QString m_functionName;
	QList<Number> m_sourcePoint;
	Number m_stopValue;
	QList<Number> m_steps;
	Number m_accelerationStep;
	Number m_decreaseStep;
};

#endif // CONFIGURATION_H
