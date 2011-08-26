#include "fibonacci.h"

namespace
{
	Fibonacci instance;
}

RpnOperand Fibonacci::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
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

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = QVariant::fromValue(findMinimum());
	return result;
}

QList<RpnArgument> Fibonacci::requiredArguments()
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
	QList<RpnOperand> functionArguments;
	RpnOperand functionArgument(RpnOperandNumber, argument);
	functionArguments << functionArgument;

	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}
