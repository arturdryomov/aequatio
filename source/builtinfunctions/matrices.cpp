#include "matrices.h"

namespace BuiltInFunctions {
namespace MatrixFunctions{

namespace
{
	MatrixDeterminant matrixDeterminant;
}

RpnOperand MatrixDeterminant::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix = RpnVector::toTwoDimensional(actualArguments.first().value.value<RpnVector>());

	// MathUtils will ensure that matrix is square
	Number result = MathUtils::countDeterminant(matrix);

	return RpnOperand(RpnOperandNumber, QVariant::fromValue(result));
}

QList<RpnArgument> MatrixDeterminant::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandVector);

	return arguments;
}


namespace
{
	MatrixSum matrixSum;
}

RpnOperand MatrixSum::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix1 = RpnVector::toTwoDimensional(actualArguments.at(0).value.value<RpnVector>());
	QList<QList<Number> > matrix2 = RpnVector::toTwoDimensional(actualArguments.at(1).value.value<RpnVector>());

	MathUtils::ensureMatrix(matrix1);
	MathUtils::ensureMatrix(matrix2);

	if (matrix1.first().size() != matrix2.first().size()) {
		THROW(ENotSameSizeMatrices());
	}

	QList<QList<Number> > sum;
	for (int i = 0; i < matrix1.size(); ++i) {
		sum << MathUtils::addVectorToVector(matrix1.at(i), matrix2.at(i));
	}

	RpnVector result = RpnVector::fromTwoDimensional(sum);
	return RpnOperand(RpnOperandVector, QVariant::fromValue(result));
}

QList<RpnArgument> MatrixSum::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
			<< RpnArgument(RpnOperandVector)
			<< RpnArgument(RpnOperandVector);

	return arguments;
}


namespace
{
	MatrixDiff matrixDiff;
}

RpnOperand MatrixDiff::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix1 = RpnVector::toTwoDimensional(actualArguments.at(0).value.value<RpnVector>());
	QList<QList<Number> > matrix2 = RpnVector::toTwoDimensional(actualArguments.at(1).value.value<RpnVector>());

	MathUtils::ensureMatrix(matrix1);
	MathUtils::ensureMatrix(matrix2);

	if (matrix1.first().size() != matrix2.first().size()) {
		THROW(ENotSameSizeMatrices());
	}

	QList<QList<Number> > diff;
	for (int i = 0; i < matrix1.size(); ++i) {
		diff << MathUtils::subtractVectorFromVector(matrix1.at(i), matrix2.at(i));
	}

	RpnVector result = RpnVector::fromTwoDimensional(diff);
	return RpnOperand(RpnOperandVector, QVariant::fromValue(result));
}

QList<RpnArgument> MatrixDiff::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
			<< RpnArgument(RpnOperandVector)
			<< RpnArgument(RpnOperandVector);

	return arguments;
}


namespace
{
	MatrixTrace matrixTrace;
}

RpnOperand MatrixTrace::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix = RpnVector::toTwoDimensional(actualArguments.at(0).value.value<RpnVector>());

	MathUtils::ensureSquareMatrix(matrix);

	Number sum = 0.0;
	for (int i = 0; i < matrix.size(); ++i) {
		sum += matrix.at(i).at(i);
	}

	return RpnOperand(RpnOperandNumber, QVariant::fromValue(sum));
}

QList<RpnArgument> MatrixTrace::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandVector);

	return arguments;
}


namespace
{
	MatrixMultiply matrixMultiply;
}

RpnOperand MatrixMultiply::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix1 = RpnVector::toTwoDimensional(actualArguments.at(0).value.value<RpnVector>());
	QList<QList<Number> > matrix2 = RpnVector::toTwoDimensional(actualArguments.at(1).value.value<RpnVector>());
	MathUtils::ensureMatrix(matrix1);
	MathUtils::ensureMatrix(matrix2);

	if (matrix1.first().size() != matrix2.size()) {
		THROW(ENotCorrespondingMatricesSizes());
	}

	QList<QList<Number> > product;
	int columnsCount = matrix2.first().size();
	int rowsCount = matrix1.size();
	int sameSize = matrix2.size();

	for (int i = 0; i < rowsCount; i++) {
		QList<Number> row;
		for (int j = 0; j < columnsCount; j++) {
			Number element = 0;
			for (int k = 0; k < sameSize; k++) {
				element += matrix1.at(i).at(k) * matrix2.at(k).at(j);
			}
			row << element;
		}
		product << row;
	}

	RpnVector result = RpnVector::fromTwoDimensional(product);
	return RpnOperand(RpnOperandVector, QVariant::fromValue(result));
}

QList<RpnArgument> MatrixMultiply::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
			<< RpnArgument(RpnOperandVector)
			<< RpnArgument(RpnOperandVector);

	return arguments;
}


namespace
{
	MatrixNormM matrixNormM;
}

RpnOperand MatrixNormM::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator)

	QList<QList<Number> > matrix = RpnVector::toTwoDimensional(actualArguments.at(0).value.value<RpnVector>());
	MathUtils::ensureMatrix(matrix);

	Number result = 0.0;

	for (int i = 0; i < matrix.size(); i++) {
		Number sum = 0.0;
		foreach (Number element, matrix.at(i)) {
			sum += qAbs(element);
		}

		// First line, remember it for comparsion
		if (i == 0) {
			result = sum;
		}
		else {
			if (sum > result) {
				result = sum;
			}
		}
	}

	return RpnOperand(RpnOperandNumber, QVariant::fromValue(result));
}

QList<RpnArgument> MatrixNormM::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandVector);

	return arguments;
}


namespace
{
	MatrixNormL matrixNormL;
}

RpnOperand MatrixNormL::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator)

	QList<QList<Number> > matrix = RpnVector::toTwoDimensional(actualArguments.at(0).value.value<RpnVector>());
	MathUtils::ensureMatrix(matrix);

	Number result = 0.0;

	for (int i = 0; i < matrix.first().size(); i++) {
		Number sum = 0.0;
		for (int j = 0; j < matrix.size(); j++) {
			sum += qAbs(matrix.at(j).at(i));
		}

		// First column, remember it for comparsion
		if (i == 0) {
			result = sum;
		}
		else {
			if (sum > result) {
				result = sum;
			}
		}
	}

	return RpnOperand(RpnOperandNumber, QVariant::fromValue(result));
}

QList<RpnArgument> MatrixNormL::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandVector);

	return arguments;
}


namespace
{
	MatrixNormFrobenius matrixNormFrobenius;
}

RpnOperand MatrixNormFrobenius::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator)

	QList<QList<Number> > matrix = RpnVector::toTwoDimensional(actualArguments.at(0).value.value<RpnVector>());
	MathUtils::ensureMatrix(matrix);

	Number result = 0.0;
	foreach (const QList<Number> &row, matrix) {
		foreach (Number element, row) {
			result += element * element;
		}
	}

	result = qSqrt(result);

	return RpnOperand(RpnOperandNumber, QVariant::fromValue(result));
}

QList<RpnArgument> MatrixNormFrobenius::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandVector);

	return arguments;
}

namespace
{
	MatrixTranspose matrixTranspose;
}

RpnOperand MatrixTranspose::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix = RpnVector::toTwoDimensional(actualArguments.at(0).value.value<RpnVector>());
	MathUtils::ensureMatrix(matrix);

	for (int i = 0; i < matrix.size(); i++) {
		for (int j = i + 1; j < matrix.at(i).size(); j++) {
			Number buffer = matrix.at(i).at(j);
			matrix[i][j] = matrix.at(j).at(i);
			matrix[j][i] = buffer;
		}
	}

	RpnVector result = RpnVector::fromTwoDimensional(matrix);
	return RpnOperand(RpnOperandVector, QVariant::fromValue(result));
}

QList<RpnArgument> MatrixTranspose::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandVector);

	return arguments;
}

namespace
{
	MatrixInverse matrixInverse;
}

RpnOperand MatrixInverse::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > initial = RpnVector::toTwoDimensional(actualArguments.at(0).value.value<RpnVector>());
	MathUtils::ensureSquareMatrix(initial);

	try {
		QList<QList<Number> > result = inverse(initial);
		return RpnOperand(RpnOperandVector, QVariant::fromValue(RpnVector::fromTwoDimensional(result)));
	}
	catch (ENoSolution &e) {
		Q_UNUSED(e)
		return RpnOperand(RpnOperandNumber, QVariant::fromValue(MathUtils::getNaN()));
	}

}

QList<QList<Number> > MatrixInverse::inverse(QList<QList<Number> > &initial)
{
	// we use Gauss–Jordan elimination method (http://en.wikipedia.org/wiki/Gauss%E2%80%93Jordan_elimination)

	QList<QList<Number> > inversed = unitaryMatrix(initial.size());
	int size = initial.size();

	// Save initial order of columns.
	QList<int> orderOfColumns;
	for (int i = 0; i < size; ++i) {
		orderOfColumns << i;
	}

	// Direct pass

	for (int i = 0; i < size; ++i) {

		int mainElementIndex = i;
		Number mainElement = initial[i][i];

		// find main element of the current row
		for (int j = i + 1; j < size; ++j) {
			if (qAbs(mainElement) < qAbs(initial[i][j])) {
				mainElementIndex = j;
				mainElement = initial[i][j];
			}
		}

		if (MathUtils::isNull(mainElement)) {
			THROW(ENoSolution());
		}

		// swap columns
		if (mainElementIndex != i) {
			swapColumns(initial, i, mainElementIndex);
			orderOfColumns.swap(i, mainElementIndex);
		}

		// divide the row by the main element
		initial[i] = MathUtils::divideVectorByNumber(initial[i], mainElement);
		inversed[i] = MathUtils::divideVectorByNumber(inversed[i], mainElement);

		// subtract current row (multiplied before) from rows below.
		for (int j = i + 1; j < size; ++j) {
			QList<Number> multipliedRow = MathUtils::multiplyVectorByNumber(initial[i], initial[j][i]);
			QList<Number> multipliedRowInversed = MathUtils::multiplyVectorByNumber(inversed[i], initial[j][i]);

			// initial
			initial[j] = MathUtils::subtractVectorFromVector(initial[j], multipliedRow);

			// inverse
			inversed[j] = MathUtils::subtractVectorFromVector(inversed[j], multipliedRowInversed);
		}
	}

	// Reverse pass

	for (int i = size - 1; i >= 0; --i) {
		for (int j = i - 1; j >= 0; --j) {

			// subtract current row (multiplied before) from rows above.

			QList<Number> multipliedRow = MathUtils::multiplyVectorByNumber(initial[i], initial[j][i]);
			QList<Number> multipliedRowInversed =MathUtils::multiplyVectorByNumber(inversed[i], initial[j][i]);

			// initial
			initial[j] = MathUtils::subtractVectorFromVector(initial[j], multipliedRow);

			// inverse
			inversed[j] = MathUtils::subtractVectorFromVector(inversed[j], multipliedRowInversed);
		}
	}

	// we need to restore the order of inversed matrix's rows -- use orderOfColumns for this
	QList<QList<Number> > result = inversed;
	for (int i = 0; i < size; ++i) {
		result[orderOfColumns[i]] = inversed[i];
	}

	return result;
}

QList<RpnArgument> MatrixInverse::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandVector);

	return arguments;
}

void MatrixInverse::swapColumns(QList<QList<Number> > &matrix, int index1, int index2)
{
	QMutableListIterator<QList<Number> > i(matrix);
	while (i.hasNext()) {
		i.next().swap(index1, index2);
	}
}

QList<QList <Number> > MatrixInverse::unitaryMatrix(int size)
{
	QList<QList <Number> > result;

	for (int i = 0; i < size; ++i) {

		QList<Number> row;
		for (int j = 0; j < size; ++j) {
			if (i == j) {
				row << 1.0;
			}
			else {
				row << 0.0;
			}
		}

		result << row;
	}

	return result;
}

} // namespace
} // namespace
