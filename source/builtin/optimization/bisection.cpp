#include "bisection.h"

namespace BuiltIn {
namespace Optimization {

namespace
{
	Bisection instance;
}

Rpn::Operand Bisection::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments[0].value.value<QString>();
	m_sourceInterval.leftBorder = actualArguments[1].value.value<Number>();
	m_sourceInterval.rightBorder = actualArguments[2].value.value<Number>();
	m_accuracy = actualArguments[3].value.value<Number>();

	// Check values of variables for currect algorithm work
	if (m_accuracy <= 0) {
		THROW(EWrongArgument(QObject::tr("accuracy"), QObject::tr("more than 0")) )
	}

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = QVariant::fromValue(findMinimum());
	return result;
}

QList<Rpn::Argument> Bisection::requiredArguments()
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

Number Bisection::findMinimum()
{
	Number middlePoint = (m_sourceInterval.leftBorder + m_sourceInterval.rightBorder) / 2;

	forever {
		Interval currentInterval = {
			m_sourceInterval.leftBorder +
				qAbs(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder) / 4,
			m_sourceInterval.rightBorder -
				qAbs(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder) / 4
		};

		if (countFunction(currentInterval.leftBorder) < countFunction(middlePoint)) {
			m_sourceInterval.rightBorder = middlePoint;
			middlePoint = currentInterval.leftBorder;
		}
		else if (countFunction(currentInterval.rightBorder) < countFunction(middlePoint)) {
				m_sourceInterval.leftBorder = middlePoint;
				middlePoint = currentInterval.rightBorder;
		}
		else {
			m_sourceInterval.leftBorder = currentInterval.leftBorder;
			m_sourceInterval.rightBorder = currentInterval.rightBorder;
		}

		if (qAbs(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder) <= m_accuracy) {
			// Exit condition
			return middlePoint;
		}
	}
}

Number Bisection::countFunction(Number argument)
{
	QList<Rpn::Operand> functionArguments;
	Rpn::Operand functionArgument(Rpn::OperandNumber, argument);
	functionArguments << functionArgument;

	Rpn::Operand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

} // namespace
} // namespace
