#include "matrices.h"

namespace BuiltIn {
namespace MatrixFunctions {

namespace
{
	MatrixDeterminant matrixDeterminant;
}

MatrixDeterminant::MatrixDeterminant() : Function("matrix_det")
{
}

Rpn::Operand MatrixDeterminant::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix = Rpn::Vector::toTwoDimensional(actualArguments.first().value.value<Rpn::Vector>());

	// MathUtils will ensure that matrix is square
	Number result = MathUtils::countDeterminant(matrix);

	return Rpn::Operand(Rpn::OperandNumber, QVariant::fromValue(result));
}

QList<Rpn::Argument> MatrixDeterminant::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType MatrixDeterminant::returnValueType()
{
	return Rpn::OperandNumber;
}


namespace
{
	MatrixSum matrixSum;
}

MatrixSum::MatrixSum() : Function("matrix_sum")
{
}

Rpn::Operand MatrixSum::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix1 = Rpn::Vector::toTwoDimensional(actualArguments.at(0).value.value<Rpn::Vector>());
	QList<QList<Number> > matrix2 = Rpn::Vector::toTwoDimensional(actualArguments.at(1).value.value<Rpn::Vector>());

	MathUtils::ensureMatrix(matrix1);
	MathUtils::ensureMatrix(matrix2);

	if (matrix1.first().size() != matrix2.first().size()) {
		THROW(ENotSameSizeMatrices());
	}

	QList<QList<Number> > sum;
	for (int i = 0; i < matrix1.size(); ++i) {
		sum << MathUtils::addVectorToVector(matrix1.at(i), matrix2.at(i));
	}

	Rpn::Vector result = Rpn::Vector::fromTwoDimensional(sum);
	return Rpn::Operand(Rpn::OperandVector, QVariant::fromValue(result));
}

QList<Rpn::Argument> MatrixSum::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
			<< Rpn::Argument(Rpn::OperandVector)
			<< Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType MatrixSum::returnValueType()
{
	return Rpn::OperandVector;
}


namespace
{
	MatrixDiff matrixDiff;
}

MatrixDiff::MatrixDiff() : Function("matrix_diff")
{
}

Rpn::Operand MatrixDiff::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix1 = Rpn::Vector::toTwoDimensional(actualArguments.at(0).value.value<Rpn::Vector>());
	QList<QList<Number> > matrix2 = Rpn::Vector::toTwoDimensional(actualArguments.at(1).value.value<Rpn::Vector>());

	MathUtils::ensureMatrix(matrix1);
	MathUtils::ensureMatrix(matrix2);

	if (matrix1.first().size() != matrix2.first().size()) {
		THROW(ENotSameSizeMatrices());
	}

	QList<QList<Number> > diff;
	for (int i = 0; i < matrix1.size(); ++i) {
		diff << MathUtils::subtractVectorFromVector(matrix1.at(i), matrix2.at(i));
	}

	Rpn::Vector result = Rpn::Vector::fromTwoDimensional(diff);
	return Rpn::Operand(Rpn::OperandVector, QVariant::fromValue(result));
}

QList<Rpn::Argument> MatrixDiff::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
			<< Rpn::Argument(Rpn::OperandVector)
			<< Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType MatrixDiff::returnValueType()
{
	return Rpn::OperandVector;
}


namespace
{
	MatrixTrace matrixTrace;
}

MatrixTrace::MatrixTrace() : Function("matrix_trace")
{
}

Rpn::Operand MatrixTrace::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix = Rpn::Vector::toTwoDimensional(actualArguments.at(0).value.value<Rpn::Vector>());

	MathUtils::ensureSquareMatrix(matrix);

	Number sum = 0.0;
	for (int i = 0; i < matrix.size(); ++i) {
		sum += matrix.at(i).at(i);
	}

	return Rpn::Operand(Rpn::OperandNumber, QVariant::fromValue(sum));
}

QList<Rpn::Argument> MatrixTrace::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType MatrixTrace::returnValueType()
{
	return Rpn::OperandNumber;
}


namespace
{
	MatrixMultiply matrixMultiply;
}

MatrixMultiply::MatrixMultiply() : Function("matrix_multiply")
{
}

Rpn::Operand MatrixMultiply::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix1 = Rpn::Vector::toTwoDimensional(actualArguments.at(0).value.value<Rpn::Vector>());
	QList<QList<Number> > matrix2 = Rpn::Vector::toTwoDimensional(actualArguments.at(1).value.value<Rpn::Vector>());
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

	Rpn::Vector result = Rpn::Vector::fromTwoDimensional(product);
	return Rpn::Operand(Rpn::OperandVector, QVariant::fromValue(result));
}

QList<Rpn::Argument> MatrixMultiply::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
			<< Rpn::Argument(Rpn::OperandVector)
			<< Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType MatrixMultiply::returnValueType()
{
	return Rpn::OperandVector;
}


namespace
{
	MatrixNormM matrixNormM;
}

MatrixNormM::MatrixNormM() : Function("matrix_norm_m")
{
}

Rpn::Operand MatrixNormM::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator)

	QList<QList<Number> > matrix = Rpn::Vector::toTwoDimensional(actualArguments.at(0).value.value<Rpn::Vector>());
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

	return Rpn::Operand(Rpn::OperandNumber, QVariant::fromValue(result));
}

QList<Rpn::Argument> MatrixNormM::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType MatrixNormM::returnValueType()
{
	return Rpn::OperandNumber;
}


namespace
{
	MatrixNormL matrixNormL;
}

MatrixNormL::MatrixNormL() : Function("matrix_norm_l")
{
}

Rpn::Operand MatrixNormL::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator)

	QList<QList<Number> > matrix = Rpn::Vector::toTwoDimensional(actualArguments.at(0).value.value<Rpn::Vector>());
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

	return Rpn::Operand(Rpn::OperandNumber, QVariant::fromValue(result));
}

QList<Rpn::Argument> MatrixNormL::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType MatrixNormL::returnValueType()
{
	return Rpn::OperandNumber;
}


namespace
{
	MatrixNormFrobenius matrixNormFrobenius;
}

MatrixNormFrobenius::MatrixNormFrobenius() : Function("matrix_norm_frobenius")
{
}

Rpn::Operand MatrixNormFrobenius::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator)

	QList<QList<Number> > matrix = Rpn::Vector::toTwoDimensional(actualArguments.at(0).value.value<Rpn::Vector>());
	MathUtils::ensureMatrix(matrix);

	Number result = 0.0;
	foreach (const QList<Number> &row, matrix) {
		foreach (Number element, row) {
			result += element * element;
		}
	}

	result = qSqrt(result);

	return Rpn::Operand(Rpn::OperandNumber, QVariant::fromValue(result));
}

QList<Rpn::Argument> MatrixNormFrobenius::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType MatrixNormFrobenius::returnValueType()
{
	return Rpn::OperandNumber;
}


namespace
{
	MatrixTranspose matrixTranspose;
}

MatrixTranspose::MatrixTranspose() : Function("matrix_transpose")
{
}

Rpn::Operand MatrixTranspose::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix = Rpn::Vector::toTwoDimensional(actualArguments.at(0).value.value<Rpn::Vector>());
	MathUtils::ensureMatrix(matrix);

	for (int i = 0; i < matrix.size(); i++) {
		for (int j = i + 1; j < matrix.at(i).size(); j++) {
			Number buffer = matrix.at(i).at(j);
			matrix[i][j] = matrix.at(j).at(i);
			matrix[j][i] = buffer;
		}
	}

	Rpn::Vector result = Rpn::Vector::fromTwoDimensional(matrix);
	return Rpn::Operand(Rpn::OperandVector, QVariant::fromValue(result));
}

QList<Rpn::Argument> MatrixTranspose::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType MatrixTranspose::returnValueType()
{
	return Rpn::OperandVector;
}


namespace
{
	MatrixInverse matrixInverse;
}

MatrixInverse::MatrixInverse() : Function("matrix_inverse")
{
}

Rpn::Operand MatrixInverse::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > initial = Rpn::Vector::toTwoDimensional(actualArguments.at(0).value.value<Rpn::Vector>());
	MathUtils::ensureSquareMatrix(initial);

	try {
		QList<QList<Number> > result = inverse(initial);
		return Rpn::Operand(Rpn::OperandVector, QVariant::fromValue(Rpn::Vector::fromTwoDimensional(result)));
	}
	catch (ENoSolution &e) {
		Q_UNUSED(e)
		return Rpn::Operand(Rpn::OperandNumber, QVariant::fromValue(MathUtils::getNaN()));
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

QList<Rpn::Argument> MatrixInverse::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType MatrixInverse::returnValueType()
{
	return Rpn::OperandVector;
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
