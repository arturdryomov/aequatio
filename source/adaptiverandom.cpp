#include "adaptiverandom.h"

#include <qmath.h>
#include <cstdlib>

namespace
{
	AdaptiveRandom instance;
}

RpnOperand AdaptiveRandom::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;
	m_functionName = actualArguments[0].value.value<QString>();

	m_sourcePoint = actualArguments[1].value.value<QList<Number> >();
	if (m_calculator->functionArguments(m_functionName).size() != m_sourcePoint.size()) {
		THROW(EWrongParametersCount(QObject::tr("Source point"), m_calculator->functionArguments(m_functionName).size()));
	}

	m_acceleration = actualArguments[2].value.value<Number>();
	m_decrease = actualArguments[3].value.value<Number>();
	m_wrongStepsCount = actualArguments[4].value.value<Number>();
	m_iterationsCount = actualArguments[5].value.value<Number>();
	m_minimumStepSize = actualArguments[6].value.value<Number>();
	m_stepSize = 1;

	// Initialize random
	srand(time(NULL));

	RpnOperand result;
	result.type = RpnOperandVector;
	result.value = QVariant::fromValue(findMinimum());
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

		QList<Number> currentPoint = sumListList(
			m_sourcePoint,
			productListNumber(
				quotientListNumber(randomPoint, modulusList(randomPoint)),
				m_stepSize
			)
		);

		if (countFunction(currentPoint) < countFunction(m_sourcePoint)) {
			QList<Number> newPoint = sumListList(
				m_sourcePoint,
				productListNumber(
					diffListList(currentPoint, m_sourcePoint),
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


QList<Number> AdaptiveRandom::productListNumber(QList<Number> list, Number number)
{
	QList<Number> result;

	foreach (Number element, list) {
		result << element * number;
	}

	return result;
}

QList<Number> AdaptiveRandom::diffListList(QList<Number> source, QList<Number> subtractin)
{
	QList<Number> result;

	for (int i = 0; i < source.size(); i++) {
		result << source[i] - subtractin[i];
	}

	return result;
}

QList<Number> AdaptiveRandom::sumListList(QList<Number> source, QList<Number> item)
{
	QList<Number> result;

	for (int i = 0; i < source.size(); i++) {
		result << source[i] + item[i];
	}

	return result;
}

Number AdaptiveRandom::productListList(QList<Number> source, QList<Number> item)
{
	Number result = 0;

	for (int i = 0; i < source.size(); i++) {
		result += source[i] * item[i];
	}

	return result;
}

Number AdaptiveRandom::modulusList(QList<Number> list)
{
	Number result = 0;

	foreach (Number element, list) {
		result += qPow(element, 2);
	}

	return qSqrt(result);
}

QList<Number> AdaptiveRandom::quotientListNumber(QList<Number> source, Number divisor)
{
	QList<Number> result;

	foreach (Number element, source) {
		result << element / divisor;
	}

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
