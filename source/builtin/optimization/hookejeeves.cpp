#include "hookejeeves.h"

namespace BuiltIn {
namespace Optimization {

namespace
{
	HookeJeeves instance;
}

HookeJeeves::HookeJeeves() :
	Function("hooke_jeeves")
{
}

Rpn::Operand HookeJeeves::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_sourcePoint = Rpn::Vector::toOneDimensional(actualArguments.at(1).value.value<Rpn::Vector>());
	m_stopValue = actualArguments.at(2).value.value<Number>();
	m_stepSizes = Rpn::Vector::toOneDimensional(actualArguments.at(3).value.value<Rpn::Vector>());
	m_accelerationCoefficient = actualArguments.at(4).value.value<Number>();
	m_decreaseCoefficient = actualArguments.at(5).value.value<Number>();

	// Check values of variables for currect algorithm work
	if (m_calculator->functionArguments(m_functionName).size() != m_sourcePoint.size()) {
		THROW(EWrongParametersCount(QObject::tr("Source point"), m_calculator->functionArguments(m_functionName).size()));
	}
	if (m_stopValue <= 0) {
		THROW(EWrongArgument(QObject::tr("stop value"), QObject::tr("more than 0")) )
	}
	if (m_calculator->functionArguments(m_functionName).size() != m_stepSizes.size()) {
		THROW(EWrongParametersCount(QObject::tr("Coordinate steps"), m_calculator->functionArguments(m_functionName).size()));
	}
	if (m_accelerationCoefficient <= 0) {
		THROW(EWrongArgument(QObject::tr("acceleration coefficient"), QObject::tr("more than 0")) )
	}
	if (m_decreaseCoefficient <= 1) {
		THROW(EWrongArgument(QObject::tr("decrease step coefficient"), QObject::tr("more than 1")) )
	}

	Rpn::Operand result;
	result.type = Rpn::OperandVector;
	result.value = QVariant::fromValue(Rpn::Vector::fromOneDimensional(findMinimum()));
	return result;
}

QList<Rpn::Argument> HookeJeeves::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
		<< Rpn::Argument(Rpn::OperandFunctionName, QString(), QVariant::fromValue(Rpn::ArbitraryArgumentsCount))
		<< Rpn::Argument(Rpn::OperandVector)
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandVector)
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}

Rpn::OperandType HookeJeeves::returnValueType()
{
	return Rpn::OperandVector;
}

QList<Number> HookeJeeves::findMinimum()
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
				currentPoint[i] = m_sourcePoint[i] + m_accelerationCoefficient * (m_sourcePoint[i] - oldSourcePoint[i]);
			}
		}
		else {
			bool isFinish = true;
			for (int i = 0; i < m_stepSizes.size(); i++) {
				if (m_stepSizes[i] > m_stopValue) {
					isFinish = false;
					m_stepSizes[i] = m_stepSizes[i] / m_decreaseCoefficient;
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

QList<Number> HookeJeeves::increaseDirection(QList<Number> point, int direction)
{
	QList<Number> result = point;
	result[direction] += m_stepSizes[direction];

	return result;
}

QList<Number> HookeJeeves::decreaseDirection(QList<Number> point, int direction)
{
	QList<Number> result = point;
	result[direction] -= m_stepSizes[direction];

	return result;
}

Number HookeJeeves::countFunction(QList<Number> arguments)
{
	QList<Rpn::Operand> functionArguments;

	foreach (Number argument, arguments) {
		Rpn::Operand functionArgument(Rpn::OperandNumber, argument);
		functionArguments << functionArgument;
	}

	Rpn::Operand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

} // namespace
} // namespace
