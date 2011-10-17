#include "pludecomposition.h"

namespace BuiltIn {
namespace EquationSolving {

namespace {
	PluDecompozition instance;
}

PluDecompozition::PluDecompozition() : Function("matrix_plu")
{
}

Rpn::Operand PluDecompozition::calculate(Function::FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > matrix = Rpn::Vector::toTwoDimensional(actualArguments[0].value.value<Rpn::Vector>());
	foreach (QList<Number> coefficientsEquation, matrix) {
		if (coefficientsEquation.size() != matrix.first().size()) {
			THROW(EWrongArgument(QObject::tr("coefficient vectors"), QObject::tr("one size")));
		}
	}
	if (MathUtils::countDeterminant(matrix) == 0) {
		THROW(EWrongArgument(QObject::tr("matrix"), QObject::tr("nonsingular")));
	}

	try {
		QList<QList<QList<Number> > > result = decompose(matrix);
		return Rpn::Operand(Rpn::OperandVector, QVariant::fromValue(Rpn::Vector::fromThreeDimensional(result)));
	} catch (ENoSolution &e) {
		Q_UNUSED(e)
		return Rpn::Operand(Rpn::OperandNumber, QVariant::fromValue(MathUtils::getNaN()));
	}
}

QList<Rpn::Argument> PluDecompozition::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType PluDecompozition::returnValueType()
{
	return Rpn::OperandVector;
}

QList<QList<QList<Number> > > PluDecompozition::decompose(QList<QList<Number> > matrix)
{
	QList<QList<Number> > shiftMatrix = emptyMatrix(matrix.size());

	// Intialize shiftMatrix
	for (int i = 0; i < matrix.size(); i++) {
		shiftMatrix[i][i] = 1;
	}

	for (int i = 0; i < matrix.size(); i++) {
		int shiftIndex;

		// Find maximum
		Number maximumElement = 0;
		for (int j = i; j < matrix.size(); j++) {
			if (qAbs(matrix.at(j).at(i)) > maximumElement) {
				maximumElement = qAbs(matrix.at(j).at(i));
				shiftIndex = j;
			}
		}

		// Signular matrix
		if (maximumElement == 0) {
			THROW(ENoSolution());
		}

		// Swap vectors in matrices
		shiftMatrix.swap(i, shiftIndex);
		// Right, we are working with matrix we have
		matrix.swap(i, shiftIndex);

		// Calculate new values for L and U
		for (int j = i + 1; j < matrix.size(); j++) {
			matrix[j][i] = matrix.at(j).at(i) / matrix.at(i).at(i);
			for (int k = i + 1; k < matrix.size(); k++) {
				matrix[j][k] = matrix.at(j).at(k) - matrix.at(j).at(i) * matrix.at(i).at(k);
			}
		}
	}

	// Package
	QList<QList<QList<Number> > > result;
	result << matrix << shiftMatrix;

	return result;
}

QList<QList<Number> > PluDecompozition::emptyMatrix(int size)
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
