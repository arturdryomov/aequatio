#include "configuration.h"

namespace
{
	Configuration instance;
}

RpnOperand Configuration::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_sourcePoint = RpnVector::toOneDimensional(actualArguments.at(1).value.value<RpnVector>());
	m_stopValue = actualArguments.at(2).value.value<Number>();
	m_steps = RpnVector::toOneDimensional(actualArguments.at(3).value.value<RpnVector>());
	m_accelerationStep = actualArguments.at(4).value.value<Number>();
	m_decreaseStep = actualArguments.at(5).value.value<Number>();

	// Check values of variables for currect algorithm work
	if (m_calculator->functionArguments(m_functionName).size() != m_sourcePoint.size()) {
		THROW(EWrongParametersCount(QObject::tr("Source point"), m_calculator->functionArguments(m_functionName).size()));
	}
	if (m_stopValue <= 0) {
		THROW(EWrongArgument(QObject::tr("stop value"), QObject::tr("more than 0")) )
	}
	if (m_calculator->functionArguments(m_functionName).size() != m_steps.size()) {
		THROW(EWrongParametersCount(QObject::tr("Coordinate steps"), m_calculator->functionArguments(m_functionName).size()));
	}
	if (m_accelerationStep <= 0) {
		THROW(EWrongArgument(QObject::tr("acceleration coefficient"), QObject::tr("more than 0")) )
	}
	if (m_decreaseStep <= 1) {
		THROW(EWrongArgument(QObject::tr("decrease step coefficient"), QObject::tr("more than 1")) )
	}

	RpnOperand result;
	result.type = RpnOperandVector;
	result.value = QVariant::fromValue(RpnVector::fromOneDimensional(findMinimum()));
	return result;
}

QList<RpnArgument> Configuration::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		 // QVariant() shows that number of arguments is not fixed
		<< RpnArgument(RpnOperandFunctionName, QString(), QVariant())
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
				currentPoint[i] = m_sourcePoint[i] + m_accelerationStep * (m_sourcePoint[i] - oldSourcePoint[i]);
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
				// Exit condition
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
