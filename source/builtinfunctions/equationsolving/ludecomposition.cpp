#include "ludecomposition.h"

namespace BuiltInFunctions {
namespace EquationSolving {

namespace {
	LuDecompozition instance;
}

RpnOperand LuDecompozition::calculate(Function::FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix = RpnVector::toTwoDimensional(actualArguments[0].value.value<RpnVector>());

	QList<QList<QList<Number> > > result = decompose(matrix);
	return RpnOperand(RpnOperandVector, QVariant::fromValue(RpnVector::fromThreeDimensional(result)));
}

QList<RpnArgument> LuDecompozition::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandVector);

	return arguments;
}

QList<QList<QList<Number> > > LuDecompozition::decompose(const QList<QList<Number> > &matrix)
{
	QList<QList<Number> > lowerTriangleMatrix = emptyMatrix(matrix.size());
	QList<QList<Number> > upperTriangleMatrix = emptyMatrix(matrix.size());

	// Just shortcut
	int matrixSize = matrix.size();

	for (int i = 0; i < matrixSize; i++) {
		lowerTriangleMatrix[i][i] = 1;

		// Fill upper matrix
		for (int j = i; j < matrixSize; j++) {
			Number sum = 0;
			for (int k = 0; k < i; k++) {
				sum += lowerTriangleMatrix.at(i).at(k) * upperTriangleMatrix.at(k).at(j);
			}
			upperTriangleMatrix[i][j] = matrix.at(i).at(j) - sum;
		}

		// Fill lower matrix
		for (int j = i + 1; j < matrixSize; j++) {
			Number sum = 0;
			for (int k = 0; k < i; k++) {
				sum += lowerTriangleMatrix.at(j).at(k) * upperTriangleMatrix.at(k).at(i);
			}
			lowerTriangleMatrix[j][i] = (matrix.at(j).at(i) - sum) / upperTriangleMatrix.at(i).at(i);
		}
	}

	// Package
	QList<QList<QList<Number> > > result;
	result << lowerTriangleMatrix << upperTriangleMatrix;

	return result;
}

QList<QList<Number> > LuDecompozition::emptyMatrix(int size)
{
	QList<QList<Number> > result;

	for (int i = 0; i < size; i++) {
		QList<Number> row;
		for (int j = 0; j < size; j++) {
			row << 0.0;
		}
		result << row;
	}

	return result;
}

} // namespace
} // namespace
