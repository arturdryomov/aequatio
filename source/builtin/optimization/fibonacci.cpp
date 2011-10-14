#include "fibonacci.h"

namespace BuiltIn {
namespace Optimization {

namespace
{
	Fibonacci instance;
}

Fibonacci::Fibonacci() :
	Function("fibonacci")
{
}

Rpn::Operand Fibonacci::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_sourceInterval.leftBorder = actualArguments.at(1).value.value<Number>();
	m_sourceInterval.rightBorder = actualArguments.at(2).value.value<Number>();
	m_resultIntervalLength = actualArguments.at(3).value.value<Number>();
	m_differenceConstant = actualArguments.at(4).value.value<Number>();
	initializeIterationsNumber();

	// Check values of variables for currect algorithm work
	if (m_resultIntervalLength <= 0) {
		THROW(EWrongArgument(QObject::tr("length of finish interval"), QObject::tr("more than 0")) )
	}
	if (m_differenceConstant <= 0) {
		THROW(EWrongArgument(QObject::tr("difference constant"), QObject::tr("more than 0")) )
	}

	Rpn::Operand result;
	result.type = Rpn::OperandNumber;
	result.value = QVariant::fromValue(findMinimum());
	return result;
}

QList<Rpn::Argument> Fibonacci::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
		// 1 is argument count in function that is passed as and argument
		<< Rpn::Argument(Rpn::OperandFunctionName, QString(), QVariant::fromValue(1))
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}

Rpn::OperandType Fibonacci::returnValueType()
{
	return Rpn::OperandNumber;
}

void Fibonacci::initializeIterationsNumber()
{
	m_iterationsNumber = 0;

	while (fibonacciNumber(m_iterationsNumber) <
		qAbs(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder) /
		m_resultIntervalLength) {

		m_iterationsNumber++;
	}
}

Number Fibonacci::fibonacciNumber(int position)
{
	switch(position) {
		case 0: {
			return 1;
		};
		case 1: {
			return 1;
		};
		default: {
			return fibonacciNumber(position - 1) + fibonacciNumber(position - 2);
		};
	};
}

Number Fibonacci::findMinimum()
{
	Interval currentInterval;

	currentInterval.leftBorder = m_sourceInterval.leftBorder +
		(fibonacciNumber(m_iterationsNumber - 2) / fibonacciNumber(m_iterationsNumber)) *
		(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder);

	currentInterval.rightBorder = m_sourceInterval.leftBorder +
		(fibonacciNumber(m_iterationsNumber - 1) / fibonacciNumber(m_iterationsNumber)) *
		(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder);

	for (int iteration = 0; iteration <= m_iterationsNumber - 3; iteration++) {
		if (countFunction(currentInterval.leftBorder) <= countFunction(currentInterval.rightBorder)) {
			m_sourceInterval.rightBorder = currentInterval.rightBorder;
			currentInterval.rightBorder = currentInterval.leftBorder;

			currentInterval.leftBorder = m_sourceInterval.leftBorder +
				(fibonacciNumber(m_iterationsNumber - iteration - 3) /
					fibonacciNumber(m_iterationsNumber - iteration - 1)) *
				(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder);
		}

		else {
			m_sourceInterval.leftBorder = currentInterval.leftBorder;
			currentInterval.leftBorder = currentInterval.rightBorder;

			currentInterval.rightBorder = m_sourceInterval.leftBorder +
				(fibonacciNumber(m_iterationsNumber - iteration - 2) /
					fibonacciNumber(m_iterationsNumber - iteration - 1)) *
				(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder);
		}
	}

	currentInterval.rightBorder = currentInterval.leftBorder + m_differenceConstant;

	if (countFunction(currentInterval.leftBorder) <= countFunction(currentInterval.rightBorder)) {
		m_sourceInterval.rightBorder = currentInterval.rightBorder;
	}
	else {
		m_sourceInterval.leftBorder = currentInterval.leftBorder;
	}

	return (m_sourceInterval.rightBorder + m_sourceInterval.leftBorder) / 2;
}

Number Fibonacci::countFunction(Number argument)
{
	QList<Rpn::Operand> functionArguments;
	Rpn::Operand functionArgument(Rpn::OperandNumber, argument);
	functionArguments << functionArgument;

	Rpn::Operand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

} // namespace
} // namespace
