#include "conjugategradient.h"

namespace BuiltIn {
namespace Optimization {

namespace
{
	ConjugateGradient instance;
}

ConjugateGradient::ConjugateGradient() :
	Function("conjugate_gradient")
{
}

Rpn::Operand ConjugateGradient::calculate(FunctionCalculator* calculator, QList<Rpn::Operand> actualArguments)
{
	m_calculator = calculator;
	m_functionName = actualArguments[0].value.value<QString>();
	m_initialPoint = Rpn::Vector::toOneDimensional(actualArguments[1].value.value<Rpn::Vector>());

	int expectedArgumentsCount = m_calculator->functionArguments(m_functionName).size();
	if (m_initialPoint.size() != expectedArgumentsCount) {
		THROW(EWrongArgument("initial point", QObject::tr("1-dimensional vector with %n argument(s)",
			0, expectedArgumentsCount)));
	}

	// Stop value
	m_accuracy = actualArguments[2].value.value<Number>();

	Rpn::Vector result = Rpn::Vector::fromOneDimensional(findMinimum());
	return Rpn::Operand(Rpn::OperandVector, QVariant::fromValue(result));
}

QList<Rpn::Argument> ConjugateGradient::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
		<< Rpn::Argument(Rpn::OperandFunctionName, QString(), QVariant::fromValue(Rpn::ArbitraryArgumentsCount)) // function
		<< Rpn::Argument(Rpn::OperandVector) // initial point
		<< Rpn::Argument(Rpn::OperandNumber); // stop value

	return arguments;
}

Rpn::OperandType ConjugateGradient::returnValueType()
{
	return Rpn::OperandVector;
}

QList<Number> ConjugateGradient::findMinimum()
{
	int argumentsCount = m_calculator->functionArguments(m_functionName).size();

	QList<QList<Number> > directions = initialDirections(argumentsCount);
	QList<Number> point = m_initialPoint;

	forever {
		bool isResultFound;
		QList<Number> nextPoint;
		QList<Number> result = searchWithDirections(directions, point, nextPoint, isResultFound);
		if (isResultFound) {
			// Exit condition
			return result;
		}

		if (MathUtils::vectorNorm(MathUtils::subtractVectorFromVector(nextPoint, point)) < m_accuracy) {
			// Exit condition
			return nextPoint;
		}

		QList<QList<Number> > newDirections = directions;
		for (int i = 1; i < newDirections.size() - 1; ++i) {
			newDirections[i] = newDirections[i + 1];
		}
		newDirections.first() = MathUtils::subtractVectorFromVector(nextPoint, point);
		newDirections.last() = newDirections.first();

		if (isDirectionsLinearlyIndependend(newDirections)) {
			directions = newDirections;
		}
		point = nextPoint;
	}
}

Number ConjugateGradient::findStep(QList<Number> point, QList<Number> direction)
{
	// We'll use bisection method for 1-dimensional optimization

	Number sourceLeftBorder = -1000.0;
	Number sourceRightBorder = 1000.0;
	Number middle = (sourceLeftBorder + sourceRightBorder) / 2;
	Number accuracy = 0.00001;

	// TODO: Call calculator with bisection method?

	forever {
		Number currentLeftBorder = sourceLeftBorder + qAbs(sourceRightBorder - sourceLeftBorder) / 4.0;
		Number currentRightBorder = sourceRightBorder + qAbs(sourceRightBorder - sourceLeftBorder) / 4.0;

		QList<Number> leftBorderPoint = MathUtils::addVectorToVector(
			point,
			MathUtils::multiplyVectorByNumber(direction, currentLeftBorder)
		);
		QList<Number> rightBorderPoint = MathUtils::addVectorToVector(
			point,
			MathUtils::multiplyVectorByNumber(direction, currentRightBorder)
		);
		QList<Number> middlePoint = MathUtils::addVectorToVector(
			point,
			MathUtils::multiplyVectorByNumber(direction, middle)
		);

		if (countFunction(leftBorderPoint) <= countFunction(middlePoint)) {
			sourceRightBorder = middle;
			middle = currentLeftBorder;
		}
		else if (countFunction(rightBorderPoint) <= countFunction(middlePoint)) {
				sourceLeftBorder = middle;
				middle = currentRightBorder;
		}
		else {
			sourceLeftBorder = currentLeftBorder;
			sourceRightBorder = currentRightBorder;
		}

		if (qAbs(sourceRightBorder - sourceLeftBorder) <= accuracy) {
			// Exit condition
			return middle;
		}
	}
}

QList<QList<Number> > ConjugateGradient::initialDirections(int dimensionsCount)
{
	QList<QList<Number> > result;

	for (int i = 0; i < dimensionsCount; ++i) {
		QList<Number> direction;
		for (int j = 0; j < dimensionsCount; ++j) {
			direction << 0.0;
		}
		direction[i] = 1.0;
		result << direction;
	}

	result.prepend(result.at(dimensionsCount - 1));

	return result;
}

/* if result is found, the following is returned:
	 - isResultFound is true;
	 - searchWithDirections() returns result;
	 - finishingPoint is not changed;

  if result is not found, the following is returned:
	 - isResultFound is false;
	 - searchWithDirections() returns QList<Number>();
	 - finishingPoint is last point tested.
*/
QList<Number> ConjugateGradient::searchWithDirections(const QList<QList<Number> > directions,
	const QList<Number> &initialPoint, QList<Number> &finishingPoint, bool &isResultFound)
{
	isResultFound = false;

	QList<Number> currentPoint = initialPoint;
	QList<Number> secondIterationPoint;
	int i = 0;
	int n = initialPoint.size();

	forever {
		Number step = findStep(currentPoint, directions[i]);
		currentPoint = MathUtils::addVectorToVector(currentPoint, MathUtils::multiplyVectorByNumber(directions[i], step));
		if (i == 1) secondIterationPoint = currentPoint;

		if (i == n - 1) {
			if (currentPoint == initialPoint) {
				isResultFound = true;
				return currentPoint;
			}
		}

		else if (i == n) {
			if (currentPoint == secondIterationPoint) {
				isResultFound = true;
				return currentPoint;
			}
			else {
				break;
			}
		}

		i++;

	}

	finishingPoint = currentPoint;
	return QList<Number>();
}

bool ConjugateGradient::isDirectionsLinearlyIndependend(const QList<QList<Number> > &directions)
{
	return !MathUtils::isNull(MathUtils::countDeterminant(directions));
}

Number ConjugateGradient::countFunction(const QList<Number> arguments)
{
	QList<Rpn::Operand> functionArguments;
	foreach (Number number, arguments) {
		functionArguments << Rpn::Operand(Rpn::OperandNumber, number);
	}

	Rpn::Operand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

} // namespace
} // namespace
