#include "goldenratio.h"

#include <qmath.h>

namespace
{
	GoldenRatio instance;
}

RpnOperand GoldenRatio::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;
	m_functionName = actualArguments[0].value.value<QString>();
	m_sourceInterval.leftBorder = actualArguments[1].value.value<Number>();
	m_sourceInterval.rightBorder = actualArguments[2].value.value<Number>();
	m_accuracy = actualArguments[3].value.value<Number>();
	m_ratio = (3 - qSqrt(5)) / 2;

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = findMinimum();
	return result;
}

QList<RpnOperand> GoldenRatio::requiredArguments()
{
	QList<RpnOperand> arguments;
	arguments << RpnOperand(RpnOperandFunctionName, QVariant())
		<< RpnOperand(RpnOperandNumber, QVariant())
		<< RpnOperand(RpnOperandNumber, QVariant())
		<< RpnOperand(RpnOperandNumber, QVariant());

	return arguments;
}

Number GoldenRatio::findMinimum()
{
  Interval newInterval;
  newInterval.leftBorder = m_sourceInterval.leftBorder + 
    m_ratio * (m_sourceInterval.rightBorder - m_sourceInterval.leftBorder);
  newInterval.rightBorder = m_sourceInterval.leftBorder + 
    m_sourceInterval.rightBorder - newInterval.leftBorder;
  double delta = 0;

  do {
    // Work with external function
    double leftFunctionValue = countFunction(newInterval.leftBorder);
    double rightFunctionValue = countFunction(newInterval.rightBorder);

		// Make less interval
    if (leftFunctionValue <= rightFunctionValue) {
      m_sourceInterval.rightBorder = newInterval.rightBorder;
      newInterval.rightBorder = newInterval.leftBorder;
      newInterval.leftBorder = m_sourceInterval.leftBorder + 
        m_sourceInterval.rightBorder - newInterval.leftBorder;
    }
    else {
      m_sourceInterval.leftBorder = newInterval.leftBorder;
      newInterval.leftBorder = newInterval.rightBorder;
      newInterval.rightBorder = m_sourceInterval.leftBorder + 
        m_sourceInterval.rightBorder - newInterval.rightBorder;
    }
    
    delta = fabs(m_sourceInterval.leftBorder - m_sourceInterval.rightBorder);
  } while (delta > m_accuracy);

  return (m_sourceInterval.leftBorder + m_sourceInterval.rightBorder) / 2;
}

Number GoldenRatio::countFunction(Number argument)
{
	QList<RpnOperand> functionArguments;
	RpnOperand functionArgument(RpnOperandNumber, argument);
	functionArguments << functionArgument;

	// TODO: Check funciton in built-in or user-defined?
	// Maybe would be better for Syntax Analyzer.
	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}
