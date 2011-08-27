#include "adaptiverandom.h"

#include <ctime>

namespace BuiltInFunctions {
namespace Optimization {

namespace
{
	AdaptiveRandom instance;
}

RpnOperand AdaptiveRandom::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_sourcePoint = RpnVector::toOneDimensional(actualArguments.at(1).value.value<RpnVector>());
	m_accelerationCoefficient = actualArguments.at(2).value.value<Number>();
	m_decreaseCoefficient = actualArguments.at(3).value.value<Number>();
	m_wrongStepsCount = actualArguments.at(4).value.value<Number>();
	m_maximumIterationsCount = actualArguments.at(5).value.value<Number>();
	m_minimumStepSize = actualArguments.at(6).value.value<Number>();
	m_stepSize = 1;

	// Check values of variables for currect algorithm work
	if (m_calculator->functionArguments(m_functionName).size() != m_sourcePoint.size()) {
		THROW(EWrongParametersCount(QObject::tr("Source point"), m_calculator->functionArguments(m_functionName).size()));
	}
	if (m_accelerationCoefficient <= 1) {
		THROW(EWrongArgument(QObject::tr("acceleration coefficient"), QObject::tr("more than 1")) )
	}
	if ((m_decreaseCoefficient <= 0) || (m_decreaseCoefficient >= 1)) {
		THROW(EWrongArgument(QObject::tr("decrease coefficient"), QObject::tr("more than 0 and less than 1")) )
	}

	// Initialize random, random number generator from MathUtils needs it
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
		// QVariant() shows that number of arguments is not fixed
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
		QList<Number> randomPoint = MathUtils::generateRandomNumbers(m_sourcePoint.size(), -1, 1);

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
					m_accelerationCoefficient
				)
			);

			if (countFunction(newPoint) < countFunction(m_sourcePoint)) {
				m_sourcePoint = newPoint;
				m_stepSize *= m_accelerationCoefficient;
				iterationCount++;

				if (iterationCount < m_maximumIterationsCount) {
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
			m_stepSize *= m_decreaseCoefficient;
			failCount = 1;
		}
	}
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

} // namespace
} // namespace
