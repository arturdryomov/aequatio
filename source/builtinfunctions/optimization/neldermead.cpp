#include "neldermead.h"

namespace BuiltInFunctions {
namespace Optimization {

namespace
{
	NelderMead instance;
}

RpnOperand NelderMead::calculate(BuiltInFunction::FunctionCalculator *calculator, QList<RpnOperand> actualArguments)
{
	m_calculator = calculator;

	m_functionName = actualArguments[0].value.value<QString>();

	QList<QList<Number> > initialSimplex = RpnVector::toTwoDimensional(actualArguments[1].value.value<RpnVector>());
	if (!isVectorCorrect(initialSimplex)) {
		int targetFunctionArgumentsCount = m_calculator->functionArguments(m_functionName).count();
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

	RpnVector result = RpnVector::fromOneDimensional(findMinimum(initialSimplex, reflectionCoefficient,
		compressionCoefficient, strainCoefficient, stopValue));

	return RpnOperand(RpnOperandVector, QVariant::fromValue(result));
}

QList<RpnArgument> NelderMead::requiredArguments()
{
	QList<RpnArgument> arguments;
	arguments
		// -1 shows that number of arguments is not fixed
		<< RpnArgument(RpnOperandFunctionName, QString(), QVariant::fromValue(-1)) // function
		<< RpnArgument(RpnOperandVector) // initial simplex
		<< RpnArgument(RpnOperandNumber)	// reflection coefficient
		<< RpnArgument(RpnOperandNumber)	// compression coefficient
		<< RpnArgument(RpnOperandNumber) // strain coefficient
		<< RpnArgument(RpnOperandNumber); // stop value

	return arguments;
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
			for (int i = 0; i < simplex.count(); ++i) {
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
	QList<RpnOperand> functionArguments;
	foreach (Number number, arguments) {
		functionArguments << RpnOperand(RpnOperandNumber, number);
	}

	RpnOperand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

int NelderMead::indexOfMinimal(const QList<Number> &values)
{
	if (values.count() <= 0) return -1;

	Number min = values[0];
	int index = 0;

	for (int i = 1; i < values.count(); ++i) {
		if (values[i] < min) {
			min = values[i];
			index = i;
		}
	}

	return index;
}

int NelderMead::indexOfMaximal(const QList<Number> &values)
{
	if (values.count() <= 0) return -1;

	Number max = values[0];
	int index = 0;

	for (int i = 1; i < values.count(); ++i) {
		if (values[i] > max) {
			max = values[i];
			index = i;
		}
	}

	return index;
}

int NelderMead::indexOfSecondMaximal(const QList<Number> &values)
{
	if (values.count() <= 1) return -1;

	Number indexOfMaximal = this->indexOfMaximal(values);

	Number secondMax = values[0];
	int index = 0;

	if (indexOfMaximal == 0) {
		secondMax = values[1];
		index = 1;
	}

	for (int i = 0; i < values.count(); ++i) {
		if ((values[i] > secondMax) && (i != indexOfMaximal)) {
			secondMax = values[i];
			index = i;
		}
	}

	return index;
}

QList<Number> NelderMead::findCenter(const QList<QList<Number> > &simplex, int worstIndex)
{
	int n = simplex[0].count();

	QList<Number> center;
	for (int i = 0; i < n; ++i) {
		center << 0;
	}

	for (int i = 0; i < simplex.count(); ++i) {
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
	sigma /= simplex.count();
	sigma = qPow(sigma, 0.5);

	bool result = sigma <= accuracy;

	return result;
}

bool NelderMead::isVectorCorrect(const QList<QList<Number> > vector)
{
	int targetFunctionArgumentCount = m_calculator->functionArguments(m_functionName).count();

	if (vector.count() != targetFunctionArgumentCount + 1) {
		return false;
	}
	foreach(const QList<Number> &point, vector) {
		if (point.count() != targetFunctionArgumentCount) {
			return false;
		}
	}

	return true;
}

} // namespace
} // namespace
