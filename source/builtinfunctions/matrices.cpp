#include "matrices.h"

namespace BuiltInFunctions {
namespace MatrixFunctions{

namespace {
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

namespace {
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

namespace {
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

namespace {
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

namespace {
//	MatrixNormM matrixNormM;
}

namespace {
//	MatrixNormN matrixNormN;
}

namespace {
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

} // namespaces
}
