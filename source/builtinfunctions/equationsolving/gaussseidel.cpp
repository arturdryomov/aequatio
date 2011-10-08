#include "gaussseidel.h"

namespace BuiltInFunctions {
namespace EquationSolving {

namespace {
	GaussSeidel instance;
}

RpnOperand GaussSeidel::calculate(Function::FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;

	QList<QList<Number> > coefficients = RpnVector::toTwoDimensional(actualArguments[0].value.value<RpnVector>());
	foreach (QList<Number> coefficientsEquation, coefficients) {
		if (coefficientsEquation.size() != coefficients.first().size()) {
			THROW(EWrongArgument(QObject::tr("coefficient vectors"), QObject::tr("one size")));
		}
	}
	Number precision = actualArguments[1].value.value<Number>();

	QList<Number> result = findSolution(coefficients, precision);
	return RpnOperand(RpnOperandVector, QVariant::fromValue(RpnVector::fromOneDimensional(result)));
}

QList<RpnArgument> GaussSeidel::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		<< RpnArgument(RpnOperandVector)
		<< RpnArgument(RpnOperandNumber);

	return arguments;
}

QList<Number> GaussSeidel::findSolution(QList<QList<Number> > coefficients, Number precision)
{
	QList<QList<Number> > workingMatrix = extractWorkingMatrix(coefficients);
	QList<Number> freeCoefficients = extractFreeCoefficients(coefficients);

	QList<Number> solution = emptyVector(workingMatrix.size());
	QList<Number> previousSolution = emptyVector(workingMatrix.size());

	do {
		for (int i = 0; i < workingMatrix.size(); i++) {
			Number sum = 0;
			for (int j = 0; j < workingMatrix.size(); j++) {
				if (j != i) {
					sum += (workingMatrix.at(i).at(j) * solution.at(j));
				}
			}

			previousSolution[i] = solution.at(i);
			solution[i] = (freeCoefficients.at(i) - sum) / workingMatrix.at(i).at(i);
		}
	} while (!isConvergenceReached(previousSolution, solution, precision));

	return solution;
}

bool GaussSeidel::isConvergenceReached(QList<Number> previousSolution, QList<Number> currentSolution, Number precision)
{
	QList<Number> differenceVector = MathUtils::subtractVectorFromVector(currentSolution, previousSolution);

	// Count norm
	Number norm = 0;
	for (int i = 0; i < differenceVector.size(); i++) {
		norm += qPow(differenceVector.at(i), 2);
	}
	norm = qSqrt(norm);

	if (norm <= precision) {
		return true;
	}
	else {
		return false;
	}
}

QList<QList<Number> > GaussSeidel::extractWorkingMatrix(QList<QList<Number> > coefficients)
{
	QList<QList<Number> > result = coefficients;

	QMutableListIterator<QList<Number> > i(result);
	while (i.hasNext()) {
		i.next().removeLast();
	}

	return result;
}

QList<Number> GaussSeidel::extractFreeCoefficients(QList<QList<Number> > coefficients)
{
	QList<Number> result;

	foreach (QList<Number> row, coefficients) {
		result << row.last();
	}

	return result;
}

QList<Number> GaussSeidel::emptyVector(int size)
{
	QList<Number> result;

	for (int i = 0; i < size; i++) {
		result << 0.0;
	}

	return result;
}

} // namespace
} // namespace
