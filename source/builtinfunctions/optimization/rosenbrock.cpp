#include "rosenbrock.h"

namespace BuiltInFunctions {
namespace Optimization {

namespace
{
	Rosenbrock instance;
}

RpnOperand Rosenbrock::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_sourcePoint = RpnVector::toOneDimensional(actualArguments.at(1).value.value<RpnVector>());
	m_stopValue = actualArguments.at(2).value.value<Number>();
	m_accelerationCoefficient = actualArguments.at(3).value.value<Number>();
	m_decreaseCoefficient = actualArguments.at(4).value.value<Number>();
	m_stepSizes = RpnVector::toOneDimensional(actualArguments.at(5).value.value<RpnVector>());
	m_maximumWrongStepsCount = actualArguments.at(6).value.value<Number>();

	// Set start directions
	m_directions.clear();
	for (int i = 0; i < m_sourcePoint.size(); i++) {
		m_directions << QList<Number>();
		for (int k = 0; k < m_sourcePoint.size(); k++) {
			if (k == i) {
				m_directions[i] << 1;
			}
			else {
				m_directions[i] << 0;
			}
		}
	}

	// Check values of variables for currect algorithm work
	if (m_calculator->functionArguments(m_functionName).size() != m_sourcePoint.size()) {
		THROW(EWrongParametersCount(QObject::tr("Source point"), m_calculator->functionArguments(m_functionName).size()));
	}
	if (m_stopValue <= 0) {
		THROW(EWrongArgument(QObject::tr("stop value"), QObject::tr("more than 0")) )
	}
	if (m_accelerationCoefficient <= 1) {
		THROW(EWrongArgument(QObject::tr("acceleration coefficient"), QObject::tr("more than 1")) )
	}
	if ((m_decreaseCoefficient <= -1) || (m_decreaseCoefficient >= 0)) {
		THROW(EWrongArgument(QObject::tr("decrease coefficient"), QObject::tr("more than -1 and less than 0")) )
	}
	if (m_calculator->functionArguments(m_functionName).size() != m_stepSizes.size()) {
		THROW(EWrongParametersCount(QObject::tr("Coordinate steps"), m_calculator->functionArguments(m_functionName).size()));
	}

	RpnOperand result;
	result.type = RpnOperandVector;
	result.value = QVariant::fromValue(RpnVector::fromOneDimensional(findMinimum()));
	return result;
}

QList<RpnArgument> Rosenbrock::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		// QVariant() shows that number of arguments is not fixed
		<< RpnArgument(RpnOperandFunctionName, QString(), QVariant())
		<< RpnArgument(RpnOperandVector)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandVector)
		<< RpnArgument(RpnOperandNumber);

	return arguments;
}

QList<Number> Rosenbrock::findMinimum()
{
	QList<Number> firstCurrentPoint = m_sourcePoint;
	QList<Number> sourceSteps = m_stepSizes;
	QList<Number> currentPoint = firstCurrentPoint;

	forever {
		forever {
			int wrongStepsCount = 0;
			for (int i = 0; i < currentPoint.size(); i++) {
				if (countFunction(increaseDirection(currentPoint, i)) < countFunction(currentPoint)) {
					currentPoint = increaseDirection(currentPoint, i);
					m_stepSizes[i] = m_stepSizes[i] * m_accelerationCoefficient;
				}
				else {
					m_stepSizes[i] = m_stepSizes[i] * m_decreaseCoefficient;
					wrongStepsCount++;
				}
			}

			if (countFunction(currentPoint) == countFunction(firstCurrentPoint)) {
				if (countFunction(currentPoint) < countFunction(m_sourcePoint)) {
					// Loop exit condition
					break;
				}
				else if (countFunction(currentPoint) == countFunction(m_sourcePoint)) {
					if (wrongStepsCount <= m_maximumWrongStepsCount) {
						bool isFinish = true;
						for (int i = 0; i < m_stepSizes.size(); i++) {
							if (qAbs(m_stepSizes[i]) > m_stopValue) {
								isFinish = false;
							}
						}
						if (isFinish) {
							// Main loop exit condition
							return m_sourcePoint;
						}
						else {
							firstCurrentPoint = currentPoint;
							continue;
						}
					}
					// Loop exit condition
					else {
						break;
					}
				}
			}
			else {
				firstCurrentPoint = currentPoint;
				continue;
			}
		}

		if (MathUtils::vectorNorm(MathUtils::subtractVectorFromVector(currentPoint, m_sourcePoint))
				<= m_stopValue) {

			// Main loop exit condition
			return currentPoint;
		}
		else {
			initializeNewDirections(getStepLengths(currentPoint, m_sourcePoint));
			m_stepSizes = sourceSteps;
			m_sourcePoint = currentPoint;
			firstCurrentPoint = currentPoint;
		}
	}
}

void Rosenbrock::initializeNewDirections(QList<Number> stepSizes)
{
	// Two steps of getting new directions by Gram algorithm

	QList<QList<Number> > gramStepOne;
	for (int i = 0; i < stepSizes.size(); i++) {
		if (stepSizes[i] == 0) {
			gramStepOne << m_directions[i];
		}
		else {
			QList<Number> element;
			for (int j = i; j < stepSizes.size(); j++) {
				if (element != QList<Number>()) {
					element = MathUtils::addVectorToVector(element,
						MathUtils::multiplyVectorByNumber(m_directions[j], stepSizes[j]));
				}
				else {
					element = MathUtils::multiplyVectorByNumber(m_directions[j], stepSizes[j]);
				}
			}
			gramStepOne << element;
		}
	}

	QList<QList<Number> > gramStepTwo;
	for (int i = 0; i < stepSizes.size(); i++) {
		QList<Number> element;

		if (i == 0) {
			element = gramStepOne[i];
		}
		else {
			QList<Number> subtractin;
			for (int j = 0; j < i; j++) {
				subtractin = MathUtils::multiplyVectorByNumber(gramStepTwo[j],
					MathUtils::multiplyVectorByVectorScalar(gramStepOne[i], gramStepTwo[j]));
				if (j != 0) {
					subtractin = MathUtils::addVectorToVector(subtractin, gramStepTwo[j]);
				}
			}

			element = MathUtils::subtractVectorFromVector(gramStepOne[i], subtractin);
		}

		m_directions[i] = MathUtils::divideVectorByNumber(element, MathUtils::vectorNorm(element));
		gramStepTwo << m_directions[i];
	}
}

QList<Number> Rosenbrock::getStepLengths(QList<Number> currentPoint, QList<Number> previousPoint)
{
	QList<QList<Number> > equationsCoefficients;
	for (int i = 0; i < currentPoint.size(); i++) {
		QList<Number> equationCoefficients;
		foreach (QList<Number> direction, m_directions) {
			equationCoefficients << direction.at(i);
		}
		equationCoefficients << MathUtils::subtractVectorFromVector(currentPoint, previousPoint).at(i);

		equationsCoefficients << equationCoefficients;
	}


	return solveEquationSystem(equationsCoefficients);
}

QList<Number> Rosenbrock::solveEquationSystem(QList<QList<Number> > coefficients)
{
	QList<RpnOperand> arguments;
	RpnOperand vectorArgument(RpnOperandVector, QVariant::fromValue(RpnVector::fromTwoDimensional(coefficients)));
	arguments << vectorArgument;

	RpnOperand result = m_calculator->calculate("cramer", arguments);
	return RpnVector::toOneDimensional(result.value.value<RpnVector>());
}

QList<Number> Rosenbrock::increaseDirection(QList<Number> point, int direction)
{
	QList<Number> result = point;

	for (int i = 0; i < point.size(); i++) {
		result[i] += m_directions[direction][i] * m_stepSizes[direction];
	}

	return result;
}

Number Rosenbrock::countFunction(QList<Number> arguments)
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
