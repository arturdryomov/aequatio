#include "rosenbrock.h"

#include <qmath.h>

namespace
{
	Rosenbrock instance;
}

RpnOperand Rosenbrock::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;

	m_functionName = actualArguments[0].value.value<QString>();

	m_sourcePoint = actualArguments[1].value.value<QList<Number> >();
	if (m_calculator->functionArguments(m_functionName).size() != m_sourcePoint.size()) {
		THROW(EWrongParametersCount(QObject::tr("Source point"), m_calculator->functionArguments(m_functionName).size()));
	}

	m_stopValue = actualArguments[2].value.value<Number>();
	m_accelerationStep = actualArguments[3].value.value<Number>();
	m_decreaseStep = actualArguments[4].value.value<Number>();

	m_steps = actualArguments[5].value.value<QList<Number> >();
	if (m_calculator->functionArguments(m_functionName).size() != m_steps.size()) {
		THROW(EWrongParametersCount(QObject::tr("Coordinate steps"), m_calculator->functionArguments(m_functionName).size()));
	}

	m_wrongStepsNumber = actualArguments[6].value.value<Number>();

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
	result.value = QVariant::fromValue(findMinimum());
	return result;
}

QList<RpnArgument> Rosenbrock::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		// NOTE: QVariant() shows that number of arguments is not fixed, maybe there is other way
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
	// Step 1
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

		Number modulus = modulusList(diffListList(currentPoint, m_sourcePoint));
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
					element = sumListList(element, productListNumber(m_directions[j], stepSizes[j]));
				}
				else {
					element = productListNumber(m_directions[j], stepSizes[j]);
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
				subtractin = productListNumber(gramStepTwo[j], productListList(gramStepOne[i], gramStepTwo[j]));
				if (j != 0) {
					subtractin = sumListList(subtractin, gramStepTwo[j]);
				}
			}

			element = diffListList(gramStepOne[i], subtractin);
		}

		m_directions[i] = quotientListNumber(element, modulusList(element));
		gramStepTwo << m_directions[i];
	}
}

QList<Number> Rosenbrock::getStepLengths(QList<Number> currentPoint, QList<Number> previousPoint)
{
	QList<QList<Number> > equationCoefficients;
	equationCoefficients << diffListList(currentPoint, previousPoint);
	foreach (QList<Number> direction, m_directions) {
		equationCoefficients << direction;
	}

	return solveEquationSystem(equationCoefficients);
}

// First list of coefficients is equations results
QList<Number> Rosenbrock::solveEquationSystem(QList<QList<Number> > coefficients)
{
	QList<Number> result;

	QVector<QVector<Number> > mainMatrix;
	for (int i = 1; i < coefficients.size(); i++) {
		mainMatrix << QVector<Number>::fromList(coefficients[i]);
	}
	Number mainDeterminant = countDeterminant(mainMatrix);


	for (int i = 1; i < coefficients.size(); i++) {
		QVector<QVector<Number> > elementMatrix;
		for (int j = 1; j < coefficients.size(); j++) {
			if (j == i) {
				elementMatrix << QVector<Number>::fromList(coefficients[0]);
			}
			else {
				elementMatrix << QVector<Number>::fromList(coefficients[j]);
			}
		}

		result << countDeterminant(elementMatrix) / mainDeterminant;
	}

	return result;
}

Number Rosenbrock::countDeterminant(QVector<QVector<Number> > matrix)
{
	Number result = 0;

	if (matrix.size() == 1) {
		result = matrix[0][0];
	}
	else if (matrix.size() == 2) {
		result = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
	}
	else {
		// Go threw first line
		for (int i = 0; i < matrix.size(); i++) {
			// Initialize minor matrix
			QVector<QVector<Number> > minorMatrix;
			minorMatrix.resize(matrix.size() - 1);
			for (int j = 0; j < minorMatrix.size(); j++) {
				minorMatrix[j].resize(matrix.size() - 1);
			}

			// Fill minor matrix
			for (int j = 0; j < matrix.size(); j++) {
				int k = 0;
				for (int l = 0; l < matrix.size(); l++) {
					// Don't conpy the minor column
					if (l != i) {
						minorMatrix[j - 1][k] = minorMatrix[j][l];
						k++;
					}
				}
			}

			result +=
				qPow(-1, 1 + i + 1) *
				minorMatrix[0][i] *
				countDeterminant(minorMatrix);
		}
	}

	return result;
}

QList<Number> Rosenbrock::increaseDirection(QList<Number> point, int direction)
{
	QList<Number> result = point;

	for (int i = 0; i < point.size(); i++) {
		result[i] += m_directions[direction][i] * m_steps[direction];
	}

	return result;
}

QList<Number> Rosenbrock::productListNumber(QList<Number> list, Number number)
{
	QList<Number> result;

	foreach (Number element, list) {
		result << element * number;
	}

	return result;
}

QList<Number> Rosenbrock::diffListList(QList<Number> source, QList<Number> subtractin)
{
	QList<Number> result;

	for (int i = 0; i < source.size(); i++) {
		result << source[i] - subtractin[i];
	}

	return result;
}

QList<Number> Rosenbrock::sumListList(QList<Number> source, QList<Number> item)
{
	QList<Number> result;

	for (int i = 0; i < source.size(); i++) {
		result << source[i] + item[i];
	}

	return result;
}

Number Rosenbrock::productListList(QList<Number> source, QList<Number> item)
{
	Number result = 0;

	for (int i = 0; i < source.size(); i++) {
		result += source[i] * item[i];
	}

	return result;
}

Number Rosenbrock::modulusList(QList<Number> list)
{
	Number result = 0;

	foreach (Number element, list) {
		result += qPow(element, 2);
	}

	return qSqrt(result);
}

QList<Number> Rosenbrock::quotientListNumber(QList<Number> source, Number divisor)
{
	QList<Number> result;

	foreach (Number element, source) {
		result << element / divisor;
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
