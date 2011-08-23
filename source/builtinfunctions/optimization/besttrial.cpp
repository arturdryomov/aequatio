#include "besttrial.h"

#include <ctime>

namespace
{
	BestTrial instance;
}

RpnOperand BestTrial::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;
	m_functionName = actualArguments[0].value.value<QString>();

	m_sourcePoint = RpnVector::toOneDimensional(actualArguments[1].value.value<RpnVector>());
	if (m_calculator->functionArguments(m_functionName).size() != m_sourcePoint.size()) {
		THROW(EWrongParametersCount(QObject::tr("Source point"), m_calculator->functionArguments(m_functionName).size()));
	}

	m_decrease = actualArguments[2].value.value<Number>();
	if ((m_decrease <= 0) || (m_decrease >= 1)) {
		THROW(EWrongArgument(QObject::tr("decrease coefficient"), QObject::tr("more than 0 and less than 1")) )
	}

	m_stepsCount = actualArguments[3].value.value<Number>();
	m_iterationsCount = actualArguments[4].value.value<Number>();
	m_minimumStepSize = actualArguments[5].value.value<Number>();
	m_stepSize = 1;

	// Initialize random
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
		// NOTE: QVariant() shows that number of arguments is not fixed, maybe there is other way
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
			randomPoints << generateRandomNumbers(m_sourcePoint.size(), -1, 1);
		}

		QList<QList<Number> > currentPoints;
		for (int i = 0; i < m_stepsCount; i++) {
			currentPoints << MathUtils::addVectorToVector(
				m_sourcePoint,
				MathUtils::multiplyVectorByNumber(
					MathUtils::divideVectorByNumber(randomPoints[i], MathUtils::vectorNorm(randomPoints[i])),
					m_stepSize
				)
			);
		}

		QList<Number> currentPoint = getSpecialMinimum(currentPoints);

		if (countFunction(currentPoint) < countFunction(m_sourcePoint)) {
			m_sourcePoint = currentPoint;

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

// TODO: Move to MathUtils
QList<Number> BestTrial::generateRandomNumbers(int count, Number lowerLimit, Number higherLimit)
{
	QList<Number> result;

	for (int i = 0; i < count; i++) {
		result << getRandomNumber(qAbs(lowerLimit - higherLimit)) + lowerLimit;
	}

	return result;
}

// Return a random number between 0 and limit
Number BestTrial::getRandomNumber(Number limit)
{
	Number result;

	do {
		result = rand() / (RAND_MAX / (limit + 1));
	} while (result > limit);

	return result;
}

QList<Number> BestTrial::getSpecialMinimum(QList<QList<Number> > points)
{
	QList<Number> result = points.first();

	Number minimumFunctionResult = countFunction(result);
	foreach (QList<Number> point, points) {
		if (countFunction(point) < minimumFunctionResult) {
			result = point;
			minimumFunctionResult = countFunction(point);
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
