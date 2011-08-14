#ifndef ROSENBROCK_H
#define ROSENBROCK_H

#include "builtinfunction.h"

// Class for finding minimum of function with Rosenbrock method
// TODO: Check it, second primer from book is not working fine, questions about 3(1) step
class Rosenbrock : public BuiltInFunction
{
public:
	Rosenbrock() : BuiltInFunction("rosenbrock")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	QList<Number> findMinimum();

	Number countFunction(QList<Number> arguments);

	QList<Number> increaseDirection(QList<Number> point, int direction);
	void getNewDirections(QList<Number> stepSizes);
	QList<Number> getStepLengths(QList<Number> currentPoint, QList<Number> previousPoint);

	Number countDeterminant(QVector<QVector<Number> > matrix);
	QList<Number> solveEquationSystem(QList<QList<Number> > coefficients);

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
	Number m_stopValue;
	Number m_accelerationStep;
	Number m_decreaseStep;
	QList<Number> m_steps;
	Number m_wrongStepsNumber;
	QList<QList<Number> > m_directions;
};

#endif // ROSENBROCK_H
