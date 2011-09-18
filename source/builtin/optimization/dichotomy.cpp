#include "dichotomy.h"

namespace BuiltIn {
namespace Optimization {

namespace
{
	Dichotomy instance;
}

RpnOperand Dichotomy::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_sourceInterval.leftBorder = actualArguments.at(1).value.value<Number>();
	m_sourceInterval.rightBorder = actualArguments.at(2).value.value<Number>();
	m_accuracy = actualArguments.at(3).value.value<Number>();
	m_swing = actualArguments.at(4).value.value<Number>();

	// Check values of variables for currect algorithm work
	if (m_accuracy <= 0) {
		THROW(EWrongArgument(QObject::tr("accuracy"), QObject::tr("more than 0")) )
	}
	if (m_swing <= 0) {
		THROW(EWrongArgument(QObject::tr("configuration small number"), QObject::tr("more than 0")) )
	}

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = QVariant::fromValue(findMinimum());
	return result;
}

QList<RpnArgument> Dichotomy::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		// 1 is argument count in function that is passed as and argument
		<< RpnArgument(RpnOperandFunctionName, QString(), QVariant::fromValue(1))
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber);

	return arguments;
}

Number Dichotomy::findMinimum()
{
	forever {
		Interval currentInterval;
		currentInterval.leftBorder = (m_sourceInterval.leftBorder +
			m_sourceInterval.rightBorder - m_swing) / 2;
		currentInterval.rightBorder = (m_sourceInterval.leftBorder +
			m_sourceInterval.rightBorder + m_swing) / 2;

		if (countFunction(currentInterval.leftBorder) <= countFunction(currentInterval.rightBorder)) {
			m_sourceInterval.rightBorder = currentInterval.rightBorder;
		}
		else {
			m_sourceInterval.leftBorder = currentInterval.leftBorder;
		}

		if (qAbs(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder) <= m_accuracy) {
			// Exit condition
			return (m_sourceInterval.leftBorder + m_sourceInterval.rightBorder) / 2;
		}
	}
}

Number Dichotomy::countFunction(Number argument)
{
	QList<RpnOperand> functionArguments;
	RpnOperand functionArgument(RpnOperandNumber, argument);
	functionArguments << functionArgument;

	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

} // namespace
} // namespace
