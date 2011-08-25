#include "besttrial.h"

#include <ctime>

namespace
{
	BestTrial instance;
}

RpnOperand BestTrial::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_sourcePoint = RpnVector::toOneDimensional(actualArguments.at(1).value.value<RpnVector>());
	m_decrease = actualArguments.at(2).value.value<Number>();
	m_stepsCount = actualArguments.at(3).value.value<Number>();
	m_iterationsCount = actualArguments.at(4).value.value<Number>();
	m_minimumStepSize = actualArguments.at(5).value.value<Number>();
	m_stepSize = 1;

	// Check values of variables for currect algorithm work
	if (m_calculator->functionArguments(m_functionName).size() != m_sourcePoint.size()) {
		THROW(EWrongParametersCount(QObject::tr("Source point"), m_calculator->functionArguments(m_functionName).size()));
	}
	if ((m_decrease <= 0) || (m_decrease >= 1)) {
		THROW(EWrongArgument(QObject::tr("decrease coefficient"), QObject::tr("more than 0 and less than 1")) )
	}

	// Initialize random, random number generator from MathUtils needs it
	srand(time(NULL));

	RpnOperand result;
	result.type = RpnOperandVector;
	result.value = QVariant::fromValue(RpnVector::fromOneDimensional(findMinimum()));
	return result;
}

QList<RpnArgument> BestTrial::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		// QVariant() shows that number of arguments is not fixed
		<< RpnArgument(RpnOperandFunctionName, QString(), QVariant())
		<< RpnArgument(RpnOperandVector)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber);

	return arguments;
}

QList<Number> BestTrial::findMinimum()
{
	int iterationCount = 0;

	forever {
		QList<QList<Number> > randomPoints;
		for (int i = 0; i < m_stepsCount; i++) {
			randomPoints << MathUtils::generateRandomNumbers(m_sourcePoint.size(), -1, 1);
		}

		QList<QList<Number> > currentPoints;
		for (int i = 0; i < m_stepsCount; i++) {
			currentPoints << MathUtils::addVectorToVector(
				m_sourcePoint,
				MathUtils::multiplyVectorByNumber(
					MathUtils::divideVectorByNumber(randomPoints.at(i), MathUtils::vectorNorm(randomPoints.at(i))),
					m_stepSize
				)
			);
		}

		QList<Number> currentPoint = getSpecialMinimum(currentPoints);

		if (countFunction(currentPoint) < countFunction(m_sourcePoint)) {
			m_sourcePoint = currentPoint;
			iterationCount++;

			if (iterationCount < m_iterationsCount) {
				continue;
			}
			else {
				// Exit condition
				return m_sourcePoint;
			}
		}


	if (m_stepSize <= m_minimumStepSize) {
			// Exit condition
			return m_sourcePoint;
		}
		else {
			m_stepSize *= m_decrease;
		}
	}
}

QList<Number> BestTrial::getSpecialMinimum(QList<QList<Number> > points)
{
	QList<Number> result = points.first();

	foreach (QList<Number> point, points) {
		if (countFunction(point) < countFunction(result)) {
			result = point;
		}
	}

	return result;
}

Number BestTrial::countFunction(QList<Number> arguments)
{
	QList<RpnOperand> functionArguments;

	foreach (Number argument, arguments) {
		RpnOperand functionArgument(RpnOperandNumber, argument);
		functionArguments << functionArgument;
	}

	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}
