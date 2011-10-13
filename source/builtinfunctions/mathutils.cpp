#include "mathutils.h"
#include <limits>

namespace BuiltInFunctions {

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
		result << source.at(i) - subtrahend.at(i);
	}

	return result;
}

QList<Number> MathUtils::addVectorToVector(QList<Number> source, QList<Number> summand)
{
	QList<Number> result;

	for (int i = 0; i < source.size(); i++) {
		result << source.at(i) + summand.at(i);
	}

	return result;
}

Number MathUtils::multiplyVectorByVectorScalar(QList<Number> source, QList<Number> factor)
{
	Number result = 0;

	for (int i = 0; i < source.size(); i++) {
		result += source.at(i) * factor.at(i);
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

Number MathUtils::countDeterminant(QList<QList<Number> > matrix)
{
	ensureSquareMatrix(matrix);

	int numberOfSwaps = 0;

	// Direct pass

	for (int i = 0; i < matrix.count(); ++i) {

		int mainElementIndex = i;
		Number mainElement = matrix[i][i];

		// find main element of the current row
		for (int j = i + 1; j < matrix.count(); ++j) {
			if (qAbs(mainElement) < qAbs(matrix[i][j])) {
				mainElementIndex = j;
				mainElement = matrix[i][j];
			}
		}

		if (MathUtils::isNull(mainElement)) {
			return 0.0;
		}

		// swap columns
		if (mainElementIndex != i) {
			swapColumns(matrix, i, mainElementIndex);
			++numberOfSwaps;
		}

		// subtract current row (multiplied before) from rows below.
		for (int j = i + 1; j < matrix.count(); ++j) {
			Number multiplyer = matrix[j][i] / matrix[i][i];
			QList<Number> multipliedRow = MathUtils::multiplyVectorByNumber(matrix[i], multiplyer);
			matrix[j] = MathUtils::subtractVectorFromVector(matrix[j], multipliedRow);
		}
	}

	// Now we've got triangular matrix, multiply all elements of its main diagonal

	Number result = 1.0;
	for (int i = 0; i < matrix.count(); ++i) {
		result *= matrix[i][i];
	}
	if (numberOfSwaps % 2 == 1) {
		result *= -1.0;
	}

	return result;
}

bool MathUtils::isBetween(Number value, Number left, Number right, bool canBeEqual)
{
	if (canBeEqual) {
		return (value >= left) && (value <= right);
	}
	else {
		return (value > left) && (value < right);
	}
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

QList<Number> MathUtils::generateRandomNumbers(int count, Number lowerLimit, Number higherLimit)
{
	QList<Number> result;

	for (int i = 0; i < count; i++) {
		result << getRandomNumber(qAbs(lowerLimit - higherLimit)) + lowerLimit;
	}

	return result;
}

// Return a random number between 0 and higher limit
// For currect using initialize random first with srand function
Number MathUtils::getRandomNumber(Number higherLimit)
{
	Number result;

	do {
		result = rand() / (RAND_MAX / (higherLimit + 1));
	} while (result > higherLimit);

	return result;
}

void MathUtils::ensureSquareMatrix(const QList<QList<Number> > &matrix)
{
	int matrixSize = matrix.size();
	foreach (const QList<Number> &row, matrix) {
		if (row.size() != matrixSize) {
			THROW(ENotSquareMatrix());
		}
	}
}

void MathUtils::ensureMatrix(const QList<QList<Number> > &matrix)
{
	int matrixSize = matrix.first().size();
	foreach (const QList<Number> &row, matrix) {
		if (row.size() != matrixSize) {
			THROW(ENotMatrix());
		}
	}
}

void MathUtils::swapColumns(QList<QList<Number> > &matrix, int index1, int index2)
{
	// Matrix 'matrix' must be really a matrix :), not just a list of list of numbers.

	if ((index1 >= matrix.first().count()) || (index2 >= matrix.first().count())) {
		THROW(EInternal());
	}

	QMutableListIterator<QList<Number> > i(matrix);
	while (i.hasNext()) {
		i.next().swap(index1, index2);
	}
}

Number MathUtils::machineEpsilon()
{
	return std::numeric_limits<Number>::epsilon();
}

} // namespace
