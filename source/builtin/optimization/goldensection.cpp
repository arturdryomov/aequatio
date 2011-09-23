#include "goldensection.h"

namespace BuiltIn {
namespace Optimization {

namespace
{
	GoldenSection instance;
}

Rpn::Operand GoldenSection::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
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

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = QVariant::fromValue(findMinimum());
	return result;
}

QList<Rpn::Argument> GoldenSection::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
		// 1 is argument count in function that is passed as and argument
		<< Rpn::Argument(Rpn::OperandFunctionName, QString(), QVariant::fromValue(1))
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandNumber);

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
	QList<Rpn::Operand> functionArguments;
	Rpn::Operand functionArgument(Rpn::OperandNumber, argument);
	functionArguments << functionArgument;

	Rpn::Operand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

} // namespace
} // namespace
