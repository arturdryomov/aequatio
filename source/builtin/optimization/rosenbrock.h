#ifndef ROSENBROCK_H
#define ROSENBROCK_H

#include "../function.h"

namespace BuiltIn {
namespace Optimization {

// Class for finding minimum of function with Rosenbrock method
// TODO: Check it, second primer from book is not working fine, questions about 3(1) step
class Rosenbrock : public Function
{
public:
	Rosenbrock() : Function("rosenbrock")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	FunctionCalculator* m_calculator;
	QString m_functionName;
	QList<Number> m_sourcePoint;
	Number m_stopValue;
	Number m_accelerationCoefficient;
	Number m_decreaseCoefficient;
	QList<Number> m_stepSizes;
	Number m_maximumWrongStepsCount;
	QList<QList<Number> > m_directions;

	QList<Number> findMinimum();
	QList<Number> increaseDirection(QList<Number> point, int direction);
	void initializeNewDirections(QList<Number> stepSizes);
	QList<Number> getStepLengths(QList<Number> currentPoint, QList<Number> previousPoint);
	Number countFunction(QList<Number> arguments);
	QList<Number> solveEquationSystem(QList<QList<Number> > coefficients);
};

} // namespace
} // namespace

#endif // ROSENBROCK_H
