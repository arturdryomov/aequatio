#include "neldermead.h"

namespace BuiltIn {
namespace Optimization {

namespace
{
	NelderMead instance;
}

Rpn::Operand NelderMead::calculate(Function::FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	m_calculator = calculator;

	m_functionName = actualArguments[0].value.value<QString>();

	QList<QList<Number> > initialSimplex = Rpn::Vector::toTwoDimensional(actualArguments[1].value.value<Rpn::Vector>());
	if (!isVectorCorrect(initialSimplex)) {
		int targetFunctionArgumentsCount = m_calculator->functionArguments(m_functionName).size();
		THROW(EWrongArgument("initial simplex", QObject::tr("%1 × %2 size")
			.arg(targetFunctionArgumentsCount + 1)
			.arg(targetFunctionArgumentsCount))
		);
	}

	Number reflectionCoefficient = actualArguments[2].value.value<Number>();
	Number compressionCoefficient = actualArguments[3].value.value<Number>();
	Number strainCoefficient = actualArguments[4].value.value<Number>();
	Number stopValue = actualArguments[5].value.value<Number>();

	if (stopValue <= 0) {
		THROW(EWrongArgument("epsilon", "greater than zero"));
	}

	Rpn::Vector result = Rpn::Vector::fromOneDimensional(findMinimum(initialSimplex, reflectionCoefficient,
		compressionCoefficient, strainCoefficient, stopValue));

	return Rpn::Operand(Rpn::OperandVector, QVariant::fromValue(result));
}

QList<Rpn::Argument> NelderMead::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
		<< Rpn::Argument(Rpn::OperandFunctionName, QString(), QVariant::fromValue(Rpn::ArbitraryArgumentsCount)) // function
		<< Rpn::Argument(Rpn::OperandVector) // initial simplex
		<< Rpn::Argument(Rpn::OperandNumber)	// reflection coefficient
		<< Rpn::Argument(Rpn::OperandNumber)	// compression coefficient
		<< Rpn::Argument(Rpn::OperandNumber) // strain coefficient
		<< Rpn::Argument(Rpn::OperandNumber); // stop value

	return arguments;
}

Rpn::OperandType NelderMead::returnValueType()
{
	return Rpn::OperandVector;
}

QList<Number> NelderMead::findMinimum(const QList<QList<Number> > &initialSimplex, Number reflectionCoefficient,
	Number contractionCoefficient, Number expansionCoefficient, Number accuracy)
{
	QList<QList<Number> > simplex = initialSimplex;
	forever {

		// find best, worst and second worst values

		QList<Number> functions;
		foreach (const QList<Number> point, simplex) {
			functions << function(point);
		}

		int best = indexOfMinimal(functions);
		int worst = indexOfMaximal(functions);
		int secondWorst = indexOfSecondMaximal(functions);

		// find center value
		QList<Number> center = findCenter(simplex, worst);

		// check for exit
		if (found(simplex, center, accuracy)) {
			return simplex[best];
		}

		// get reflected value

		QList<Number> reflected = MathUtils::addVectorToVector(
			center,
			MathUtils::multiplyVectorByNumber(
				MathUtils::subtractVectorFromVector(
					center, simplex[worst]),
				reflectionCoefficient));

		// reflected <= best

		if (function(reflected) <= function(simplex[best])) {

			QList<Number> expanded = MathUtils::addVectorToVector(
				center,
				MathUtils::multiplyVectorByNumber(
					MathUtils::subtractVectorFromVector(
						reflected, center),
					expansionCoefficient)
			);

			if (function(expanded) < function(simplex[best])) {
				simplex[worst] = expanded;
			}
			else {
				simplex[worst] = reflected;
			}
		}

		// second-worst < reflected <= worst

		else if (MathUtils::isBetween(function(reflected), function(simplex[secondWorst]), function(simplex[worst]), true)) {
			QList<Number> contracted = MathUtils::addVectorToVector(
				center,
				MathUtils::multiplyVectorByNumber(
					MathUtils::subtractVectorFromVector(
						simplex[worst], center),
					contractionCoefficient)
			);

			simplex[worst] = contracted;
		}

		// best < reflected <= worst

		else if (MathUtils::isBetween(function(reflected), function(simplex[best]), function(simplex[worst]), true)) {
			simplex[worst] = reflected;
		}

		// reflected > worst
		else {
			QList<Number> bestValue = simplex[best];
			for (int i = 0; i < simplex.size(); ++i) {
				simplex[i] = MathUtils::addVectorToVector(
					bestValue,
					MathUtils::multiplyVectorByNumber(
						MathUtils::subtractVectorFromVector(
							simplex[i], bestValue),
						0.5)
				);
			}
		}
	}
}

Number NelderMead::function(const QList<Number> &arguments)
{
	QList<Rpn::Operand> functionArguments;
	foreach (Number number, arguments) {
		functionArguments << Rpn::Operand(Rpn::OperandNumber, number);
	}

	Rpn::Operand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

int NelderMead::indexOfMinimal(const QList<Number> &values)
{
	if (values.size() <= 0) return -1;

	Number min = values[0];
	int index = 0;

	for (int i = 1; i < values.size(); ++i) {
		if (values[i] < min) {
			min = values[i];
			index = i;
		}
	}

	return index;
}

int NelderMead::indexOfMaximal(const QList<Number> &values)
{
	if (values.size() <= 0) return -1;

	Number max = values[0];
	int index = 0;

	for (int i = 1; i < values.size(); ++i) {
		if (values[i] > max) {
			max = values[i];
			index = i;
		}
	}

	return index;
}

int NelderMead::indexOfSecondMaximal(const QList<Number> &values)
{
	if (values.size() <= 1) return -1;

	Number indexOfMaximal = this->indexOfMaximal(values);

	Number secondMax = values[0];
	int index = 0;

	if (indexOfMaximal == 0) {
		secondMax = values[1];
		index = 1;
	}

	for (int i = 0; i < values.size(); ++i) {
		if ((values[i] > secondMax) && (i != indexOfMaximal)) {
			secondMax = values[i];
			index = i;
		}
	}

	return index;
}

QList<Number> NelderMead::findCenter(const QList<QList<Number> > &simplex, int worstIndex)
{
	int n = simplex[0].size();

	QList<Number> center;
	for (int i = 0; i < n; ++i) {
		center << 0;
	}

	for (int i = 0; i < simplex.size(); ++i) {
		if (i == worstIndex) continue;
		center = MathUtils::addVectorToVector(center, simplex[i]);
	}

	center = MathUtils::multiplyVectorByNumber(center, 1.0 / n);

	return center;
}

bool NelderMead::found(const QList<QList<Number> > &simplex, const QList<Number> &center, Number accuracy)
{
	Number sigma = 0;

	foreach (const QList<Number> point, simplex) {
		Number diff = qPow(function(point) - function(center), 2);
		sigma += diff;
	}
	sigma /= simplex.size();
	sigma = qPow(sigma, 0.5);

	bool result = sigma <= accuracy;

	return result;
}

bool NelderMead::isVectorCorrect(const QList<QList<Number> > vector)
{
	int targetFunctionArgumentCount = m_calculator->functionArguments(m_functionName).size();

	if (vector.size() != targetFunctionArgumentCount + 1) {
		return false;
	}
	foreach(const QList<Number> &point, vector) {
		if (point.size() != targetFunctionArgumentCount) {
			return false;
		}
	}

	return true;
}

} // namespace
} // namespace
