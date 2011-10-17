#include "gaussseidel.h"

namespace BuiltIn {
namespace EquationSolving {

namespace {
	GaussSeidel instance;
}

GaussSeidel::GaussSeidel() : Function("sle_gauss_seidel")
{
}

Rpn::Operand GaussSeidel::calculate(Function::FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	m_calculator = calculator;

	QList<QList<Number> > coefficients = Rpn::Vector::toTwoDimensional(actualArguments[0].value.value<Rpn::Vector>());
	foreach (QList<Number> coefficientsEquation, coefficients) {
		if (coefficientsEquation.size() != coefficients.first().size()) {
			THROW(EWrongArgument(QObject::tr("coefficient vectors"), QObject::tr("one size")));
		}
	}
	Number precision = actualArguments[1].value.value<Number>();

	try {
		QList<Number> result = findSolution(coefficients, precision);
		return Rpn::Operand(Rpn::OperandVector, QVariant::fromValue(Rpn::Vector::fromOneDimensional(result)));
	} catch (ENoSolution &e) {
		Q_UNUSED(e)
		return Rpn::Operand(Rpn::OperandNumber, QVariant::fromValue(MathUtils::getNaN()));
	}
}

QList<Rpn::Argument> GaussSeidel::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
		<< Rpn::Argument(Rpn::OperandVector)
		<< Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}

Rpn::OperandType GaussSeidel::returnValueType()
{
	return Rpn::OperandVector;
}

QList<Number> GaussSeidel::findSolution(QList<QList<Number> > coefficients, Number precision)
{
	QList<QList<Number> > workingMatrix = extractWorkingMatrix(coefficients);
	QList<Number> freeCoefficients = extractFreeCoefficients(coefficients);

	if (!hasSolution(workingMatrix)) {
		THROW(ENoSolution());
	}

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

bool GaussSeidel::hasSolution(QList<QList<Number> > matrix)
{
	// Check for diagonally dominant matrix

	for (int i = 0; i < matrix.size(); i++) {
		Number sum = 0;
		for (int j = 0; j < matrix.size(); j++) {
			if (i != j) {
				sum += qAbs(matrix.at(i).at(j));
			}
		}

		// Use strict comparsion (non-strict cause infinite loop)
		if (qAbs(matrix.at(i).at(i)) <= sum) {
			return false;
		}
	}

	return true;
}

} // namespace
} // namespace
