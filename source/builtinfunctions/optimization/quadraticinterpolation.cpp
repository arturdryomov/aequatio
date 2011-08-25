#include "quadraticinterpolation.h"

namespace
{
	QuadraticInterpolation instance;
}

RpnOperand QuadraticInterpolation::calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_startPoint = actualArguments.at(1).value.value<Number>();
	m_stepSize = actualArguments.at(2).value.value<Number>();
	m_firstAccuracy = actualArguments.at(3).value.value<Number>();
	m_secondAccuracy = actualArguments.at(4).value.value<Number>();

	// Check values of variables for currect algorithm work
	if (m_stepSize <= 0) {
		THROW(EWrongArgument(QObject::tr("step size"), QObject::tr("more than 0")) )
	}

	RpnOperand result;
	result.type = RpnOperandNumber;
	result.value = QVariant::fromValue(findMinimum());
	return result;
}

QList<RpnArgument> QuadraticInterpolation::requiredArguments()
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

Number QuadraticInterpolation::findMinimum()
{
	bool needInitializePoints = true;

	forever {
		Number secondPoint, thirdPoint;

		if (needInitializePoints) {
			secondPoint = m_startPoint + m_stepSize;

			if (countFunction(m_startPoint) > countFunction(secondPoint)) {
				thirdPoint = m_startPoint + 2 * m_stepSize;
			}
			else {
				thirdPoint = m_startPoint - m_stepSize;
			}
		}

		Number minimumPoint = minimumPointValue(m_startPoint, secondPoint, thirdPoint);

		Number numerator =
			(qPow(secondPoint, 2) - qPow(thirdPoint, 2)) * countFunction(m_startPoint) +
			(qPow(thirdPoint, 2) - qPow(m_startPoint, 2)) * countFunction(secondPoint) +
			(qPow(m_startPoint, 2) - qPow(secondPoint, 2)) * countFunction(thirdPoint);

		Number denominator =
			((secondPoint - thirdPoint) * countFunction(m_startPoint)) +
			((thirdPoint - m_startPoint) * countFunction(secondPoint)) +
			((m_startPoint - secondPoint) * countFunction(thirdPoint));

		if (denominator == 0) {
			m_startPoint = minimumPoint;

			needInitializePoints = true;
			continue;
		}

		Number quadraticPoint = (0.5 * numerator) / denominator;

		if ( (qAbs((countFunction(minimumPoint) - countFunction(quadraticPoint)) / countFunction(quadraticPoint))
			< m_firstAccuracy) && (qAbs((minimumPoint - quadraticPoint) / quadraticPoint) < m_secondAccuracy) ) {
			// Exit condition
			return quadraticPoint;
		}
		else if ((quadraticPoint > m_startPoint) && (quadraticPoint < thirdPoint)) {
			QList<Number> points;
			points << m_startPoint << secondPoint << thirdPoint;
			qSort(points);

			if (minimumPoint > quadraticPoint) {
				secondPoint = quadraticPoint;
			}
			else {
				secondPoint = minimumPoint;
			}

			points.removeOne(secondPoint);
			m_startPoint = points[0];
			thirdPoint = points[1];

			needInitializePoints = false;
		}
		else {
			m_startPoint = quadraticPoint;

			needInitializePoints = true;
		}
	}
}

Number QuadraticInterpolation::minimumPointValue(Number first, Number second, Number third)
{
	Number minimumNumber = first;
	if (countFunction(minimumNumber) > countFunction(second)) {
		minimumNumber = second;
	}
	if (countFunction(minimumNumber) > countFunction(third)) {
		minimumNumber = third;
	}

	return minimumNumber;
}

Number QuadraticInterpolation::countFunction(Number argument)
{
	QList<RpnOperand> functionArguments;
	RpnOperand functionArgument(RpnOperandNumber, argument);
	functionArguments << functionArgument;

	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}
