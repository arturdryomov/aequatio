#include "cramer.h"

namespace BuiltIn {
namespace EquationSolving {

namespace
{
	Cramer instance;
}

Cramer::Cramer() :
	Function("cramer")
{
}

Rpn::Operand Cramer::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	m_matrixCoefficients = Rpn::Vector::toTwoDimensional(actualArguments[0].value.value<Rpn::Vector>());
	foreach (QList<Number> coefficientsEquation, m_matrixCoefficients) {
		if (coefficientsEquation.size() != m_matrixCoefficients.first().size()) {
			THROW(EWrongArgument(QObject::tr("coefficient vectors"), QObject::tr("one size")));
		}
	}

	try {
		QList<Number> result = findSolution();
		return Rpn::Operand(Rpn::OperandVector, QVariant::fromValue(Rpn::Vector::fromOneDimensional(result)));
	} catch (ENoSolution &e) {
		Q_UNUSED(e)
		return Rpn::Operand(Rpn::OperandNumber, QVariant::fromValue(MathUtils::getNaN()));
	}
}

QList<Rpn::Argument> Cramer::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments << Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType Cramer::returnValueType()
{
	return Rpn::OperandVector;
}

QList<Number> Cramer::findSolution()
{
	QList<Number> result;

	QVector<QVector<Number> > mainMatrix;
	foreach (QList<Number> equationCoefficients, m_matrixCoefficients) {
		// Remove result column
		equationCoefficients.removeLast();
		mainMatrix << QVector<Number>::fromList(equationCoefficients);
	}
	Number mainDeterminant = MathUtils::countDeterminant(mainMatrix);

	// System has no solution with null main determinant
	if (MathUtils::isNull(mainDeterminant)) {
		THROW(ENoSolution());
	}

	for (int i = 0; i < m_matrixCoefficients.size(); i++) {
		QVector<QVector<Number> > variableMatrix;

		foreach (QList<Number> equationCoefficients, m_matrixCoefficients) {
			QVector<Number> coefficientsRow;
			for (int j = 0; j < equationCoefficients.size() - 1; j++) {
				if (j == i) {
					// Replace variable column with result column
					coefficientsRow << equationCoefficients.last();
				}
				else {
					coefficientsRow << equationCoefficients.at(j);
				}
			}
			variableMatrix << coefficientsRow;
		}

		// Count variable result and push it
		result << MathUtils::countDeterminant(variableMatrix) / mainDeterminant;
	}

	return result;
}

} // namespace
} // namespace
