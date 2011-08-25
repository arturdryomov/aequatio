#include "cramer.h"

namespace
{
	Cramer instance;
}

RpnOperand Cramer::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	m_matrixCoefficients = RpnVector::toTwoDimensional(actualArguments[0].value.value<RpnVector>());
	foreach (QList<Number> coefficientsEquation, m_matrixCoefficients) {
		if (coefficientsEquation.size() != m_matrixCoefficients.first().size()) {
			THROW(EWrongArgument(QObject::tr("coefficient vectors"), QObject::tr("one size")));
		}
	}

	try {
		QList<Number> result = findSolution();
		return RpnOperand(RpnOperandVector, QVariant::fromValue(RpnVector::fromOneDimensional(result)));
	} catch (ENoSolution &e) {
		Q_UNUSED(e)
		return RpnOperand(RpnOperandNumber, QVariant::fromValue(MathUtils::getNaN()));
	}
}

QList<RpnArgument> Cramer::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments << RpnArgument(RpnOperandVector);

	return arguments;
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
