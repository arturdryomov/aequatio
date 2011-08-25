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
	m_difference = actualArguments.at(4).value.value<Number>();
	initializeIterationsNumber();

	// Check values of variables for currect algorithm work
	if (m_resultIntervalLength <= 0) {
		THROW(EWrongArgument(QObject::tr("length of finish interval"), QObject::tr("more than 0")) )
	}
	if (m_difference <= 0) {
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
	Interval newInterval;

	newInterval.leftBorder = m_sourceInterval.leftBorder +
		(fibonacciNumber(m_iterationsNumber - 2) / fibonacciNumber(m_iterationsNumber)) *
		(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder);

	newInterval.rightBorder = m_sourceInterval.leftBorder +
		(fibonacciNumber(m_iterationsNumber - 1) / fibonacciNumber(m_iterationsNumber)) *
		(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder);

	for (int iteration = 0; iteration <= m_iterationsNumber - 3; iteration++) {
		if (countFunction(newInterval.leftBorder) <= countFunction(newInterval.rightBorder)) {
			m_sourceInterval.rightBorder = newInterval.rightBorder;
			newInterval.rightBorder = newInterval.leftBorder;

			newInterval.leftBorder = m_sourceInterval.leftBorder +
				(fibonacciNumber(m_iterationsNumber - iteration - 3) /
					fibonacciNumber(m_iterationsNumber - iteration - 1)) *
				(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder);
		}

		else {
			m_sourceInterval.leftBorder = newInterval.leftBorder;
			newInterval.leftBorder = newInterval.rightBorder;

			newInterval.rightBorder = m_sourceInterval.leftBorder +
				(fibonacciNumber(m_iterationsNumber - iteration - 2) /
					fibonacciNumber(m_iterationsNumber - iteration - 1)) *
				(m_sourceInterval.rightBorder - m_sourceInterval.leftBorder);
		}
	}

	newInterval.rightBorder = newInterval.leftBorder + m_difference;

	if (countFunction(newInterval.leftBorder) <= countFunction(newInterval.rightBorder)) {
		m_sourceInterval.rightBorder = newInterval.rightBorder;
	}
	else {
		m_sourceInterval.leftBorder = newInterval.leftBorder;
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
