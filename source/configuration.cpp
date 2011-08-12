#include "configuration.h"

#include <qmath.h>

namespace
{
	Configuration instance;
}

RpnOperand Configuration::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;

	m_functionName = actualArguments[0].value.value<QString>();
	m_sourcePoint = actualArguments[1].value.value<QList<Number> >();	

	if (m_calculator->functionArguments(m_functionName).size() != m_sourcePoint.size()) {
		THROW(EWrongParametersCount(QObject::tr("Source point"), m_calculator->functionArguments(m_functionName).size()));
	}

	m_stopValue = actualArguments[2].value.value<Number>();
	m_steps = actualArguments[3].value.value<QList<Number> >();

	if (m_calculator->functionArguments(m_functionName).size() != m_steps.size()) {
		THROW(EWrongParametersCount(QObject::tr("Coordinate steps"), m_calculator->functionArguments(m_functionName).size()));
	}

	m_accelerationStep = actualArguments[4].value.value<Number>();
	m_decreaseStep = actualArguments[5].value.value<Number>();

	RpnOperand result;
	result.type = RpnOperandVector;
	result.value = QVariant::fromValue(findMinimum());
	return result;
}

QList<RpnArgument> Configuration::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		// 2 is argument count in function that is passed as and argument
		// TODO: Fix it, function can pass any arguments you like
		<< RpnArgument(RpnOperandFunctionName, QString(), QVariant::fromValue(2))
		<< RpnArgument(RpnOperandVector)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandVector)
		<< RpnArgument(RpnOperandNumber)
		<< RpnArgument(RpnOperandNumber);

	return arguments;
}

QList<Number> Configuration::findMinimum()
{
	QList<Number> currentPoint = m_sourcePoint;

	forever {
		// TODO: Maybe must be iterators
		for (int i = 0; i < currentPoint.size(); i++) {
			if (countFunction(increaseDirection(currentPoint, i)) < countFunction(currentPoint)) {
				currentPoint = increaseDirection(currentPoint, i);
			}
			else if (countFunction(decreaseDirection(currentPoint, i)) < countFunction(currentPoint)) {
				currentPoint = decreaseDirection(currentPoint, i);
			}
		}

		if (countFunction(currentPoint) < countFunction(m_sourcePoint)) {
			QList<Number> oldSourcePoint = m_sourcePoint;
			m_sourcePoint = currentPoint;
			for (int i = 0; i < currentPoint.size(); i++) {
				currentPoint[i] =m_sourcePoint[i] + m_accelerationStep * (m_sourcePoint[i] - oldSourcePoint[i]);
			}
		}
		else {
			bool isFinish = true;
			for (int i = 0; i < m_steps.size(); i++) {
				if (m_steps[i] > m_stopValue) {
					isFinish = false;

					m_steps[i] = m_steps[i] / m_decreaseStep;
				}
			}

			if (isFinish) {
				// Loop exit condition
				return m_sourcePoint;
			}
			else {
				currentPoint = m_sourcePoint;
			}
		}
	}
}

QList<Number> Configuration::increaseDirection(QList<Number> point, int direction)
{
	QList<Number> result = point;
	result[direction] += m_steps[direction];

	return result;
}

QList<Number> Configuration::decreaseDirection(QList<Number> point, int direction)
{
	QList<Number> result = point;
	result[direction] -= m_steps[direction];

	return result;
}

// TODO: This method is more common, it would be better for going to parent class
// TODO: If going to parent class, change prototype parametrs to (FunctionCalculator*, QList<Number)
Number Configuration::countFunction(QList<Number> arguments)
{
	QList<RpnOperand> functionArguments;

	foreach (Number argument, arguments) {
		RpnOperand functionArgument(RpnOperandNumber, argument);
		functionArguments << functionArgument;
	}

	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}
