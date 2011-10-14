#include "gauss.h"

namespace BuiltIn {
namespace EquationSolving {

namespace {
	Gauss instance;
}

Gauss::Gauss() :
	Function("gauss")
{
}

Rpn::Operand Gauss::calculate(Function::FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	QList<QList<Number> > coefficients = Rpn::Vector::toTwoDimensional(actualArguments[0].value.value<Rpn::Vector>());
	if (!areCoefficientsCorrect(coefficients)) {
		THROW(EWrongArgument("matrix", "n × (n + 1) size"));
	}

	try {
		QList<Number> result = findSolution(coefficients);
		return Rpn::Operand(Rpn::OperandVector, QVariant::fromValue(Rpn::Vector::fromOneDimensional(result)));
	} catch (ENoSolution &e) {
		Q_UNUSED(e)
		return Rpn::Operand(Rpn::OperandNumber, QVariant::fromValue(MathUtils::getNaN()));
	}
}

QList<Rpn::Argument> Gauss::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType Gauss::returnValueType()
{
	return Rpn::OperandVector;
}

QList<Number> Gauss::findSolution(QList<QList<Number> > coefficients)
{
	int variablesCount = coefficients.size();
	int columnsCount = variablesCount + 1;
	int rowsCount = variablesCount;

	// Save initial order of variables.
	QList<int> orderOfVariables;
	for (int i = 0; i < variablesCount; ++i) {
		orderOfVariables << i;
	}


	// Direct pass

	for (int i = 0; i < rowsCount; ++i) {

		int mainElementIndex = i;
		Number mainElement = coefficients[i][i];

		// Find main element of the current row
		for (int j = i + 1; j < columnsCount - 1; ++j) {
			if (qAbs(mainElement) < qAbs(coefficients[i][j])) {
				mainElementIndex = j;
				mainElement = coefficients[i][j];
			}
		}

		if (MathUtils::isNull(mainElement)) {
			THROW(ENoSolution());
		}

		// Swap columns
		if (mainElementIndex != i) {
			swapColumns(coefficients, i, mainElementIndex);
			orderOfVariables.swap(i, mainElementIndex);
		}

		// Divide the row by the main element
		coefficients[i] = MathUtils::divideVectorByNumber(coefficients[i], mainElement);

		// Subtract current row (multiplied before) from rows below.
		for (int j = i + 1; j < rowsCount; ++j) {
			QList<Number> multipliedRow = MathUtils::multiplyVectorByNumber(coefficients[i], coefficients[j][i]);
			coefficients[j] = MathUtils::subtractVectorFromVector(coefficients[j], multipliedRow);
		}
	}


	// Reverse pass

	for (int i = rowsCount - 1; i >= 0; --i) {
		for (int j = i - 1; j >= 0; --j) {
			// subtract current row (multiplied before) from rows above.
			QList<Number> multipliedRow = MathUtils::multiplyVectorByNumber(coefficients[i], coefficients[j][i]);
			coefficients[j] = MathUtils::subtractVectorFromVector(coefficients[j], multipliedRow);
		}
	}


	// Form result

	QList<Number> result = sizedVector(variablesCount);
	for (int i = 0; i < rowsCount; ++i) {
		result[orderOfVariables[i]] = coefficients[i][columnsCount - 1];
	}
	return result;
}

void Gauss::swapColumns(QList<QList<Number> > &matrix, int index1, int index2)
{
	QMutableListIterator<QList<Number> > i(matrix);
	while (i.hasNext()) {
		i.next().swap(index1, index2);
	}
}

QList<Number> Gauss::sizedVector(int size)
{
	QList<Number> result;
	for (int i = 0; i < size; ++i) {
		result << 0.0;
	}

	return result;
}

bool Gauss::areCoefficientsCorrect(const QList<QList<Number> > &coefficients)
{
	// The matrix should has N rows and N+1 columns
	int rowsCount = coefficients.size();
	foreach (const QList<Number> &row, coefficients) {
		if (row.size() != rowsCount + 1) {
			return false;
		}
	}

	return true;
}

} // namespace
} // namespace
