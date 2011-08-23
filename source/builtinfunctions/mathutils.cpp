#include "mathutils.h"
#include <limits>

QList<Number> MathUtils::multiplyVectorByNumber(QList<Number> vector, Number number)
{
	QList<Number> result;

	foreach (Number element, vector) {
		result << element * number;
	}

	return result;
}

QList<Number> MathUtils::subtractVectorFromVector(QList<Number> source, QList<Number> subtrahend)
{
	QList<Number> result;

	for (int i = 0; i < source.size(); i++) {
		result << source[i] - subtrahend[i];
	}

	return result;
}

QList<Number> MathUtils::addVectorToVector(QList<Number> source, QList<Number> summand)
{
	QList<Number> result;

	for (int i = 0; i < source.size(); i++) {
		result << source[i] + summand[i];
	}

	return result;
}

Number MathUtils::multiplyVectorByVectorScalar(QList<Number> source, QList<Number> factor)
{
	Number result = 0;

	for (int i = 0; i < source.size(); i++) {
		result += source[i] * factor[i];
	}

	return result;
}

Number MathUtils::vectorNorm(QList<Number> vector)
{
	Number result = 0;

	foreach (Number element, vector) {
		result += qPow(element, 2);
	}

	return qSqrt(result);
}

QList<Number> MathUtils::divideVectorByNumber(QList<Number> vector, Number divisor)
{
	QList<Number> result;

	foreach (Number element, vector) {
		result << element / divisor;
	}

	return result;
}

Number MathUtils::countDeterminant(QVector<QVector<Number> > matrix)
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
			for (int j = 1; j < matrix.size(); j++) {
				int k = 0;
				for (int l = 0; l < matrix.size(); l++) {
					// Don't copy the minor column
					if (l != i) {
						minorMatrix[j - 1][k] = matrix[j][l];
						k++;
					}
				}
			}

			result +=
				qPow(-1, 1 + i + 1) *
				matrix[0][i] *
				countDeterminant(minorMatrix);
		}
	}

	return result;
}

bool MathUtils::equal(Number number1, Number number2)
{
	return qFuzzyCompare(number1, number2);
}

bool MathUtils::isNull(Number number)
{
	return qFuzzyCompare(number + 1.0, 1.0); // see qFuzzyCompare(double, double) documentation
}

Number MathUtils::getNaN()
{
	return std::numeric_limits<Number>::quiet_NaN();
}

bool MathUtils::isNaN(const Number number)
{
	volatile Number value = number;
	return value != value;
}
