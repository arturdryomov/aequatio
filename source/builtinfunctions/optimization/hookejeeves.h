#ifndef HOOKEJEEVES_H
#define HOOKEJEEVES_H

#include "../builtinfunction.h"

// Class for finding minimum of function with Hooke-Jeeves method
class HookeJeeves : public BuiltInFunction
{
public:
	HookeJeeves() : BuiltInFunction("hooke_jeeves")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	FunctionCalculator* m_calculator;
	QString m_functionName;
	QList<Number> m_sourcePoint;
	Number m_stopValue;
	QList<Number> m_stepSizes;
	Number m_accelerationCoefficient;
	Number m_decreaseCoefficient;

	QList<Number> findMinimum();
	QList<Number> increaseDirection(QList<Number> point, int direction);
	QList<Number> decreaseDirection(QList<Number> point, int direction);
	Number countFunction(QList<Number> arguments);
};

#endif // HOOKEJEEVES_H
