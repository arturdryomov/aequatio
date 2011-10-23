#include "rosenbrock.h"

namespace BuiltIn {
namespace Optimization {

namespace
{
	Rosenbrock instance;
}

Rosenbrock::Rosenbrock() :
	Function("rosenbrock")
{
}

Rpn::Operand Rosenbrock::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_sourcePoint = Rpn::Vector::toOneDimensional(actualArguments.at(1).value.value<Rpn::Vector>());
	m_stopValue = actualArguments.at(2).value.value<Number>();
	m_accelerationCoefficient = actualArguments.at(3).value.value<Number>();
	m_decreaseCoefficient = actualArguments.at(4).value.value<Number>();
	m_stepSizes = Rpn::Vector::toOneDimensional(actualArguments.at(5).value.value<Rpn::Vector>());
	m_maximumWrongStepsCount = actualArguments.at(6).value.value<Number>();

	// Set start directions
	m_directions.clear();
	for (int i = 0; i < m_sourcePoint.size(); i++) {
		m_directions << QList<Number>();
		for (int k = 0; k < m_sourcePoint.size(); k++) {
			if (k == i) {
				m_directions[i] << 1;
			}
			else {
				m_directions[i] << 0;
			}
		}
	}

	// Check values of variables for currect algorithm work
	if (m_calculator->functionArguments(m_functionName).size() != m_sourcePoint.size()) {
		THROW(EWrongArgumentsCount(QObject::tr("Source point"), m_calculator->functionArguments(m_functionName).size()));
	}
	if (m_stopValue <= 0) {
		THROW(EWrongArgument(QObject::tr("stop value"), QObject::tr("more than 0")) )
	}
	if (m_accelerationCoefficient <= 1) {
		THROW(EWrongArgument(QObject::tr("acceleration coefficient"), QObject::tr("more than 1")) )
	}
	if ((m_decreaseCoefficient <= -1) || (m_decreaseCoefficient >= 0)) {
		THROW(EWrongArgument(QObject::tr("decrease coefficient"), QObject::tr("more than -1 and less than 0")) )
	}
	if (m_calculator->functionArguments(m_functionName).size() != m_stepSizes.size()) {
		THROW(EWrongArgumentsCount(QObject::tr("Coordinate steps"), m_calculator->functionArguments(m_functionName).size()));
	}

	Rpn::Operand result;
	result.type = Rpn::OperandVector;
	result.value = QVariant::fromValue(Rpn::Vector::fromOneDimensional(findMinimum()));
	return result;
}

QList<Rpn::Argument> Rosenbrock::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
		<< Rpn::Argument(Rpn::OperandFunctionName, QString(), QVariant::fromValue(Rpn::ArbitraryArgumentsCount))
		<< Rpn::Argument(Rpn::OperandVector)
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandVector)
		<< Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}

Rpn::OperandType Rosenbrock::returnValueType()
{
	return Rpn::OperandVector;
}

QList<Number> Rosenbrock::findMinimum()
{
	QList<Number> firstCurrentPoint = m_sourcePoint;
	QList<Number> sourceSteps = m_stepSizes;
	QList<Number> currentPoint = firstCurrentPoint;

	forever {
		forever {
			int wrongStepSeriesCount = 0;
			for (int i = 0; i < currentPoint.size(); i++) {
				if (calculateFunction(increaseDirection(currentPoint, i)) < calculateFunction(currentPoint)) {
					currentPoint = increaseDirection(currentPoint, i);
					m_stepSizes[i] = m_stepSizes[i] * m_accelerationCoefficient;
				}
				else {
					m_stepSizes[i] = m_stepSizes[i] * m_decreaseCoefficient;
				}
			}

			if (MathUtils::equal(calculateFunction(currentPoint), calculateFunction(firstCurrentPoint))) {
				wrongStepSeriesCount++;
				if (calculateFunction(currentPoint) < calculateFunction(m_sourcePoint)) {
					// Loop exit condition
					break;
				}
				else if (MathUtils::equal(calculateFunction(currentPoint), calculateFunction(m_sourcePoint))) {
					if (wrongStepSeriesCount <= m_maximumWrongStepsCount) {
						bool isFinish = true;
						for (int i = 0; i < m_stepSizes.size(); i++) {
							if (qAbs(m_stepSizes[i]) > m_stopValue) {
								isFinish = false;
							}
						}
						if (isFinish) {
							// Main loop exit condition
							return m_sourcePoint;
						}
						else {
							firstCurrentPoint = currentPoint;
							continue;
						}
					}
					// Loop exit condition
					else {
						break;
					}
				}
			}
			else {
				firstCurrentPoint = currentPoint;
				continue;
			}
		}

		if (MathUtils::vectorNorm(MathUtils::subtractVectorFromVector(currentPoint, m_sourcePoint))
				<= m_stopValue) {

			// Main loop exit condition
			return currentPoint;
		}
		else {
			initializeNewDirections(getStepLengths(currentPoint, m_sourcePoint));
			m_stepSizes = sourceSteps;
			m_sourcePoint = currentPoint;
			firstCurrentPoint = currentPoint;
		}
	}
}

void Rosenbrock::initializeNewDirections(QList<Number> stepSizes)
{
	// Two steps of getting new directions by Gram algorithm

	QList<QList<Number> > gramStepOne;
	for (int i = 0; i < stepSizes.size(); i++) {
		if (MathUtils::isNull(stepSizes[i])) {
			gramStepOne << m_directions[i];
		}
		else {
			QList<Number> element;
			for (int j = i; j < stepSizes.size(); j++) {
				if (element != QList<Number>()) {
					element = MathUtils::addVectorToVector(element,
						MathUtils::multiplyVectorByNumber(m_directions[j], stepSizes[j]));
				}
				else {
					element = MathUtils::multiplyVectorByNumber(m_directions[j], stepSizes[j]);
				}
			}
			gramStepOne << element;
		}
	}

	QList<QList<Number> > gramStepTwo;
	for (int i = 0; i < stepSizes.size(); i++) {
		QList<Number> element;

		if (i == 0) {
			element = gramStepOne[i];
		}
		else {
			QList<Number> subtractin;
			for (int j = 0; j < i; j++) {
				subtractin = MathUtils::multiplyVectorByNumber(gramStepTwo[j],
					MathUtils::multiplyVectorByVectorScalar(gramStepOne[i], gramStepTwo[j]));
				if (j != 0) {
					subtractin = MathUtils::addVectorToVector(subtractin, gramStepTwo[j]);
				}
			}

			element = MathUtils::subtractVectorFromVector(gramStepOne[i], subtractin);
		}

		m_directions[i] = MathUtils::divideVectorByNumber(element, MathUtils::vectorNorm(element));
		gramStepTwo << m_directions[i];
	}
}

QList<Number> Rosenbrock::getStepLengths(QList<Number> currentPoint, QList<Number> previousPoint)
{
	QList<QList<Number> > equationsCoefficients;
	for (int i = 0; i < currentPoint.size(); i++) {
		QList<Number> equationCoefficients;
		foreach (QList<Number> direction, m_directions) {
			equationCoefficients << direction.at(i);
		}
		equationCoefficients << MathUtils::subtractVectorFromVector(currentPoint, previousPoint).at(i);

		equationsCoefficients << equationCoefficients;
	}


	return solveEquationSystem(equationsCoefficients);
}

QList<Number> Rosenbrock::solveEquationSystem(QList<QList<Number> > coefficients)
{
	QList<Rpn::Operand> arguments;
	Rpn::Operand vectorArgument(Rpn::OperandVector, QVariant::fromValue(Rpn::Vector::fromTwoDimensional(coefficients)));
	arguments << vectorArgument;

	Rpn::Operand result = m_calculator->calculate("sle_gauss", arguments);
	return Rpn::Vector::toOneDimensional(result.value.value<Rpn::Vector>());
}

QList<Number> Rosenbrock::increaseDirection(QList<Number> point, int direction)
{
	QList<Number> result = point;

	for (int i = 0; i < point.size(); i++) {
		result[i] += m_directions[direction][i] * m_stepSizes[direction];
	}

	return result;
}

Number Rosenbrock::calculateFunction(QList<Number> arguments)
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
