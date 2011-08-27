#include "goldensection.h"

namespace BuiltInFunctions {
namespace Optimization {

namespace
{
	GoldenSection instance;
}

RpnOperand GoldenSection::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_sourceInterval.leftBorder = actualArguments.at(1).value.value<Number>();
	m_sourceInterval.rightBorder = actualArguments.at(2).value.value<Number>();
	m_accuracy = actualArguments.at(3).value.value<Number>();
	// Golden Ratio
	m_ratio = (3 - qSqrt(5)) / 2;

	// Check values of variables for currect algorithm work
	if (m_accuracy <= 0) {
		THROW(EWrongArgument(QObject::tr("accuracy"), QObject::tr("more than 0")) )
	}

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = QVariant::fromValue(findMinimum());
	return result;
}

QList<RpnArgument> GoldenSection::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		// 1 is argument count in function that is passed as and argument
		<< RpnArgument(RpnOperandFunctionName, QString(), QVariant::fromValue(1))
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber);

	return arguments;
}

Number GoldenSection::findMinimum()
{
	Interval currentInterval;
	currentInterval.leftBorder = m_sourceInterval.leftBorder +
		m_ratio * (m_sourceInterval.rightBorder - m_sourceInterval.leftBorder);
	currentInterval.rightBorder = m_sourceInterval.leftBorder +
		m_sourceInterval.rightBorder - currentInterval.leftBorder;
	Number delta = 0;

	do {
		if (countFunction(currentInterval.leftBorder) <= countFunction(currentInterval.rightBorder)) {
			m_sourceInterval.rightBorder = currentInterval.rightBorder;
			currentInterval.rightBorder = currentInterval.leftBorder;
			currentInterval.leftBorder = m_sourceInterval.leftBorder +
				m_sourceInterval.rightBorder - currentInterval.leftBorder;
		}
		else {
			m_sourceInterval.leftBorder = currentInterval.leftBorder;
			currentInterval.leftBorder = currentInterval.rightBorder;
			currentInterval.rightBorder = m_sourceInterval.leftBorder +
				m_sourceInterval.rightBorder - currentInterval.rightBorder;
		}

		delta = fabs(m_sourceInterval.leftBorder - m_sourceInterval.rightBorder);
	} while (delta > m_accuracy);

	return (m_sourceInterval.leftBorder + m_sourceInterval.rightBorder) / 2;
}

Number GoldenSection::countFunction(Number argument)
{
	QList<RpnOperand> functionArguments;
	RpnOperand functionArgument(RpnOperandNumber, argument);
	functionArguments << functionArgument;

	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

} // namespace
} // namespace
