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

	QList<QList<Number> > lowerTriangleMatrix = decompose(workingMatrix).at(0);
	QList<QList<Number> > upperTriangleMatrix = decompose(workingMatrix).at(1);


	QList<Number> substitutionResult = emptyVector(workingMatrix.size());

	substitutionResult.first() = freeCoefficients.first() / lowerTriangleMatrix.first().first();
	for (int i = 0; i < workingMatrix.size(); i++) {
		Number sum = 0;
		for (int j = 0; j < i; j++) {
			sum += lowerTriangleMatrix.at(i).at(j) * substitutionResult.at(j);
		}

		substitutionResult[i] = (1 / lowerTriangleMatrix.at(i).at(i)) *
			(freeCoefficients.at(i) - sum);
	}

	QList<Number> result = emptyVector(workingMatrix.size());

	result.last() = substitutionResult.last() / upperTriangleMatrix.last().last();
	for (int i = workingMatrix.size() - 1; i >= 0; i--) {
		Number sum = 0;
		for (int j = i + 1; j < workingMatrix.size(); j++) {
			sum += upperTriangleMatrix.at(i).at(j) * result.at(j);
		}

		result[i] = (1 / upperTriangleMatrix.at(i).at(i)) *
			(substitutionResult.at(i) - sum);
	}

	return result;
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

	RpnOperand result = m_calculator->calculate("lu", functionArguments);
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
