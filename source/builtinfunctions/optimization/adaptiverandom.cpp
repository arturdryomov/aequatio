#include "adaptiverandom.h"

#include <ctime>

namespace
{
	AdaptiveRandom instance;
}

RpnOperand AdaptiveRandom::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;
	m_functionName = actualArguments[0].value.value<QString>();

	m_sourcePoint = RpnVector::toOneDimensional(actualArguments[1].value.value<RpnVector>());
	if (m_calculator->functionArguments(m_functionName).size() != m_sourcePoint.size()) {
		THROW(EWrongParametersCount(QObject::tr("Source point"), m_calculator->functionArguments(m_functionName).size()));
	}

	m_acceleration = actualArguments[2].value.value<Number>();
	if (m_acceleration <= 1) {
		THROW(EWrongArgument(QObject::tr("acceleration coefficient"), QObject::tr("more than 1")) )
	}

	m_decrease = actualArguments[3].value.value<Number>();
	if ((m_decrease <= 0) || (m_decrease >= 1)) {
		THROW(EWrongArgument(QObject::tr("decrease coefficient"), QObject::tr("more than 0 and less than 1")) )
	}

	m_wrongStepsCount = actualArguments[4].value.value<Number>();
	m_iterationsCount = actualArguments[5].value.value<Number>();
	m_minimumStepSize = actualArguments[6].value.value<Number>();
	m_stepSize = 1;

	// Initialize random
	srand(time(NULL));

	RpnOperand result;
	result.type = RpnOperandVector;
	result.value = QVariant::fromValue(RpnVector::fromOneDimensional(findMinimum()));
	return result;
}

QList<RpnArgument> AdaptiveRandom::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		// NOTE: QVariant() shows that number of arguments is not fixed, maybe there is other way
		<< RpnArgument(RpnOperandFunctionName, QString(), QVariant())
		<< RpnArgument(RpnOperandVector)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber);

	return arguments;
}

QList<Number> AdaptiveRandom::findMinimum()
{
	int failCount = 1;
	int iterationCount = 0;

	forever {
		QList<Number> randomPoint = generateRandomNumbers(m_sourcePoint.size(), -1, 1);

		QList<Number> currentPoint = MathUtils::addVectorToVector(
			m_sourcePoint,
			MathUtils::multiplyVectorByNumber(
				MathUtils::divideVectorByNumber(randomPoint, MathUtils::vectorNorm(randomPoint)),
				m_stepSize
			)
		);

		if (countFunction(currentPoint) < countFunction(m_sourcePoint)) {
			QList<Number> newPoint = MathUtils::addVectorToVector(
				m_sourcePoint,
				MathUtils::multiplyVectorByNumber(
					MathUtils::subtractVectorFromVector(currentPoint, m_sourcePoint),
					m_acceleration
				)
			);

			if (countFunction(newPoint) < countFunction(m_sourcePoint)) {
				m_sourcePoint = newPoint;
				m_stepSize *= m_acceleration;
				iterationCount++;

				if (iterationCount < m_iterationsCount) {
					failCount = 1;
					continue;
				}
				else {
					// Exit condition
					return m_sourcePoint;
				}
			}
		}

		if (failCount < m_wrongStepsCount) {
			failCount++;
		}
		else if (m_stepSize <= m_minimumStepSize) {
			// Exit condition
			return m_sourcePoint;
		}
		else {
			m_stepSize *= m_decrease;
			failCount = 1;
		}
	}
}


QList<Number> AdaptiveRandom::generateRandomNumbers(int count, Number lowerLimit, Number higherLimit)
{
	QList<Number> result;

	for (int i = 0; i < count; i++) {
		result << getRandomNumber(qAbs(lowerLimit - higherLimit)) + lowerLimit;
	}

	return result;
}

// Return a random number between 0 and limit
Number AdaptiveRandom::getRandomNumber(Number limit)
{
	Number result;

	do {
		result = rand() / (RAND_MAX / (limit + 1));
	} while (result > limit);

	return result;
}


Number AdaptiveRandom::countFunction(QList<Number> arguments)
{
	QList<RpnOperand> functionArguments;

	foreach (Number argument, arguments) {
		RpnOperand functionArgument(RpnOperandNumber, argument);
		functionArguments << functionArgument;
	}

	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}
