#include "cramer.h"

namespace
{
	Cramer instance;
}

RpnOperand Cramer::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	Q_UNUSED(calculator);

	m_coefficientsMatrix = RpnVector::toTwoDimensional(actualArguments[0].value.value<RpnVector>());
	foreach (QList<Number> coefficientsEquation, m_coefficientsMatrix) {
		if (coefficientsEquation.size() != m_coefficientsMatrix.first().size()) {
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
	foreach (QList<Number> coefficientsEquation, m_coefficientsMatrix) {
		coefficientsEquation.removeLast();
		mainMatrix << QVector<Number>::fromList(coefficientsEquation);
	}
	Number mainDeterminant = MathUtils::countDeterminant(mainMatrix);

	if (MathUtils::isNull(mainDeterminant)) {
		THROW(ENoSolution());
	}

	for (int i = 0; i < m_coefficientsMatrix.size(); i++) {
		QVector<QVector<Number> > elementMatrix;

		foreach (QList<Number> coefficientsEquation, m_coefficientsMatrix) {
			QVector<Number> coefficientsRow;
			for (int j = 0; j < coefficientsEquation.size() - 1; j++) {
				if (j == i) {
					coefficientsRow << coefficientsEquation.last();
				}
				else {
					coefficientsRow << coefficientsEquation.at(j);
				}
			}
			elementMatrix << coefficientsRow;
		}

		result << MathUtils::countDeterminant(elementMatrix) / mainDeterminant;
	}

	return result;
}


