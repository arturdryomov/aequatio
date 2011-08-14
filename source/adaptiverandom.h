#ifndef ADAPTIVE_RANDOM_H
#define ADAPTIVE_RANDOM_H

#include "builtinfunction.h"

// Class for finding minimum of function with Adaptive Random method
// NOTE: This method is exprerimental, may not work at all
class AdaptiveRandom : public BuiltInFunction
{
public:
	AdaptiveRandom() : BuiltInFunction("adaptive_random")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	QList<Number> findMinimum();
	Number countFunction(QList<Number> arguments);

	QList<Number> generateRandomNumbers(int number, Number lowerLimit, Number higherLimit);
	Number getRandomNumber(Number limit);

	// List operatins
	QList<Number> productListNumber(QList<Number> list, Number number);
	QList<Number> diffListList(QList<Number> source, QList<Number> subtractin);
	QList<Number> sumListList(QList<Number> source, QList<Number> item);
	Number productListList(QList<Number> source, QList<Number> item);
	Number modulusList(QList<Number> list);
	QList<Number> quotientListNumber(QList<Number> source, Number divisor);

	FunctionCalculator* m_calculator;
	QString m_functionName;
	QList<Number> m_sourcePoint;
	Number m_acceleration;
	Number m_decrease;
	Number m_wrongStepsNumber;
	Number m_iterationsNumber;
	Number m_minimumStepSize;
	Number m_stepSize;
};

#endif // ADAPTIVE_RANDOM_H
