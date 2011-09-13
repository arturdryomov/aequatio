#include "matrices.h"

namespace BuiltInFunctions {
namespace BasicFunctions{

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
	MatrixNormM matrixNormM;
}

namespace {
	MatrixNormN matrixNormN;
}

}
}
