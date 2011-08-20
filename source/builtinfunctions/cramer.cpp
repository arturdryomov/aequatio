#include "cramer.h"

namespace
{
	Cramer instance;
}

RpnOperand Cramer::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	m_coefficientsMatrix = RpnVector::extractDoubleVector(actualArguments[0].value.value<RpnVector>());
	foreach (QList<Number> coefficientsList, m_coefficientsMatrix) {
		if (coefficientsList.size() != m_coefficientsMatrix.first().size()) {
			THROW(EWrongArgument(QObject::tr("coefficient vectors"), QObject::tr("one size")));
		}
	}

	RpnOperand result;
	result.type = RpnOperandVector;
	result.value = QVariant::fromValue(RpnVector::packageSingleVector(solveEquationSystem()));
	return result;
}


QList<RpnArgument> Cramer::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		<< RpnArgument(RpnOperandVector);

	return arguments;
}


// First list of coefficients is equations results
QList<Number> Cramer::solveEquationSystem()
{
	QList<Number> result;

	QVector<QVector<Number> > mainMatrix;
	for (int i = 1; i < m_coefficientsMatrix.size(); i++) {
		mainMatrix << QVector<Number>::fromList(m_coefficientsMatrix[i]);
	}
	Number mainDeterminant = MathUtils::countDeterminant(mainMatrix);

	for (int i = 1; i < m_coefficientsMatrix.size(); i++) {
		QVector<QVector<Number> > elementMatrix;
		for (int j = 1; j < m_coefficientsMatrix.size(); j++) {
			if (j == i) {
				elementMatrix << QVector<Number>::fromList(m_coefficientsMatrix[0]);
			}
			else {
				elementMatrix << QVector<Number>::fromList(m_coefficientsMatrix[j]);
			}
		}

		result << MathUtils::countDeterminant(elementMatrix) / mainDeterminant;
	}

	return result;
}
