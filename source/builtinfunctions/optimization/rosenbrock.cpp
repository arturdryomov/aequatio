#include "rosenbrock.h"

namespace
{
	Rosenbrock instance;
}

RpnOperand Rosenbrock::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;

	m_functionName = actualArguments[0].value.value<QString>();

	m_sourcePoint = RpnVector::toOneDimensional(actualArguments[1].value.value<RpnVector>());
	if (m_calculator->functionArguments(m_functionName).size() != m_sourcePoint.size()) {
		THROW(EWrongParametersCount(QObject::tr("Source point"), m_calculator->functionArguments(m_functionName).size()));
	}

	m_stopValue = actualArguments[2].value.value<Number>();
	if (m_stopValue <= 0) {
		THROW(EWrongArgument(QObject::tr("stop value"), QObject::tr("more than 0")) )
	}

	m_accelerationStep = actualArguments[3].value.value<Number>();
	if (m_accelerationStep <= 1) {
		THROW(EWrongArgument(QObject::tr("acceleration coefficient"), QObject::tr("more than 1")) )
	}

	m_decreaseStep = actualArguments[4].value.value<Number>();
	if ((m_decreaseStep <= -1) || (m_decreaseStep >= 0)) {
		THROW(EWrongArgument(QObject::tr("decrease coefficient"), QObject::tr("more than -1 and less than 0")) )
	}

	m_steps = RpnVector::toOneDimensional(actualArguments[5].value.value<RpnVector>());
	if (m_calculator->functionArguments(m_functionName).size() != m_steps.size()) {
		THROW(EWrongParametersCount(QObject::tr("Coordinate steps"), m_calculator->functionArguments(m_functionName).size()));
	}

	m_wrongStepsNumber = actualArguments[6].value.value<Number>();

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

	RpnOperand result;
	result.type = RpnOperandVector;
	result.value = QVariant::fromValue(RpnVector::fromOneDimensional(findMinimum()));
	return result;
}

QList<RpnArgument> Rosenbrock::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
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
	QList<Number> sourceSteps = m_steps;
	QList<Number> currentPoint = firstCurrentPoint;

	forever {
		forever {
			int failStepsCount = 0;
			for (int i = 0; i < currentPoint.size(); i++) {
				if (countFunction(increaseDirection(currentPoint, i)) < countFunction(currentPoint)) {
					currentPoint = increaseDirection(currentPoint, i);
					m_steps[i] = m_steps[i] * m_accelerationStep;
				}
				else {
					m_steps[i] = m_steps[i] * m_decreaseStep;
					failStepsCount++;
				}
			}

			if (countFunction(currentPoint) == countFunction(firstCurrentPoint)) {
				if (countFunction(currentPoint) < countFunction(m_sourcePoint)) {
					// Loop exit condition
					break;
				}
				else if (countFunction(currentPoint) == countFunction(m_sourcePoint)) {
					if (failStepsCount <= m_wrongStepsNumber) {
						bool isFinish = true;
						for (int i = 0; i < m_steps.size(); i++) {
							if (qAbs(m_steps[i]) > m_stopValue) {
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

		Number modulus = MathUtils::vectorNorm(MathUtils::subtractVectorFromVector(currentPoint, m_sourcePoint));
		if (modulus <= m_stopValue) {
			// Main loop exit condition
			return currentPoint;
		}
		else {
			getNewDirections(getStepLengths(currentPoint, m_sourcePoint));
			m_steps = sourceSteps;
			m_sourcePoint = currentPoint;
			firstCurrentPoint = currentPoint;
		}
	}
}


void Rosenbrock::getNewDirections(QList<Number> stepSizes)
{
	QList<QList<Number> > gramStepOne;
	for (int i = 0; i < stepSizes.size(); i++) {
		if (stepSizes[i] == 0) {
			gramStepOne << m_directions[i];
		}
		else {
			QList<Number> element;
			for (int j = i; j < stepSizes.size(); j++) {
				if (element != QList<Number>()) {
					element = MathUtils::addVectorToVector(element, MathUtils::multiplyVectorByNumber(m_directions[j], stepSizes[j]));
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
				subtractin = MathUtils::multiplyVectorByNumber(gramStepTwo[j], MathUtils::multiplyVectorByVectorScalar(gramStepOne[i], gramStepTwo[j]));
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


// First list of coefficients is equations results
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
		result[i] += m_directions[direction][i] * m_steps[direction];
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
