#include "lusolving.h"

namespace BuiltInFunctions {
namespace EquationSolving {

namespace {
	LuSolving instance;
}

RpnOperand LuSolving::calculate(Function::FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;

	QList<QList<Number> > coefficients = RpnVector::toTwoDimensional(actualArguments[0].value.value<RpnVector>());

	QList<Number> result = findSolution(coefficients);
	return RpnOperand(RpnOperandVector, QVariant::fromValue(RpnVector::fromOneDimensional(result)));
}

QList<RpnArgument> LuSolving::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandVector);

	return arguments;
}

QList<Number> LuSolving::findSolution(QList<QList<Number> > coefficients)
{
	QList<QList<Number> > workingMatrix = extractWorkingMatrix(coefficients);
	QList<Number> freeCoefficients = extractFreeCoefficients(coefficients);

	QList<QList<Number> > LuMatrix = decompose(workingMatrix).first();
	QList<QList<Number> > shiftMatrix = decompose(workingMatrix).last();

	// Forward substitution
	QList<Number> forwardSubstitutionResult = emptyVector(freeCoefficients.size());
	for (int i = 0; i < LuMatrix.size(); i++) {
		// Find sum
		Number sum = 0;
		for (int j = 0; j < i; j++) {
			sum += LuMatrix.at(i).at(j) * forwardSubstitutionResult.at(j);
		}

		forwardSubstitutionResult[i] =
			MathUtils::multiplyVectorByVectorScalar(freeCoefficients, shiftMatrix.at(i)) - sum;
	}

	// Back substitution
	QList<Number> backSubstitutionResult = emptyVector(freeCoefficients.size());
	for (int i = LuMatrix.size() - 1; i >= 0; i--) {
		// Find sum
		Number sum = 0;
		for (int j = i + 1; j < LuMatrix.size(); j++) {
			sum += LuMatrix.at(i).at(j) * backSubstitutionResult.at(j);
		}

		backSubstitutionResult[i] = (forwardSubstitutionResult.at(i) - sum) / LuMatrix.at(i).at(i);
	}

	return backSubstitutionResult;
}

QList<QList<Number> > LuSolving::extractWorkingMatrix(QList<QList<Number> > coefficients)
{
	QList<QList<Number> > result = coefficients;

	QMutableListIterator<QList<Number> > i(result);
	while (i.hasNext()) {
		i.next().removeLast();
	}

	return result;
}

QList<Number> LuSolving::extractFreeCoefficients(QList<QList<Number> > coefficients)
{
	QList<Number> result;

	foreach (QList<Number> row, coefficients) {
		result << row.last();
	}

	return result;
}

QList<QList<QList<Number> > > LuSolving::decompose(QList<QList<Number> > matrix)
{
	QList<RpnOperand> functionArguments;
	RpnOperand argument = RpnOperand(RpnOperandVector, QVariant::fromValue(RpnVector::fromTwoDimensional(matrix)));
	functionArguments << argument;

	// Use PLU decomposition
	RpnOperand result = m_calculator->calculate("plu", functionArguments);
	return RpnVector::toThreeDimensional(result.value.value<RpnVector>());
}

QList<Number> LuSolving::emptyVector(int size)
{
	QList<Number> result;

	for (int i = 0; i < size; i++) {
		result << 0.0;
	}

	return result;
}

} // namespace
} // namespace
