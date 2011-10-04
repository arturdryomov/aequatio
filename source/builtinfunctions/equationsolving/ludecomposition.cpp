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
	foreach (QList<Number> coefficientsEquation, matrix) {
		if (coefficientsEquation.size() != matrix.first().size()) {
			THROW(EWrongArgument(QObject::tr("coefficient vectors"), QObject::tr("one size")));
		}
	}
	if (MathUtils::countDeterminant(matrix) == 0) {
		THROW(EWrongArgument(QObject::tr("matrix"), QObject::tr("nonsingular")));
	}

	QList<QList<Number> > result = decompose(matrix);
	return RpnOperand(RpnOperandVector, QVariant::fromValue(RpnVector::fromTwoDimensional(result)));
}

QList<RpnArgument> LuDecompozition::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandVector);

	return arguments;
}

QList<QList<Number> > LuDecompozition::decompose(QList<QList<Number> > matrix)
{
	for (int i = 0; i < matrix.size(); i++) {
		matrix[i][i] = matrix.at(i).at(i);

		for (int j = i + 1; j < matrix.size(); j++) {
			matrix[j][i] = matrix.at(j).at(i) / matrix.at(i).at(i);
			matrix[i][j] = matrix.at(i).at(j);
		}

		// Schur complement
		for (int j = i + 1; j < matrix.size(); j++) {
			for (int k = i + 1; k < matrix.size(); k++) {
				matrix[j][k] = matrix.at(j).at(k) - matrix.at(j).at(i) * matrix.at(i).at(k);
			}
		}
	}

	return matrix;
}

} // namespace
} // namespace
