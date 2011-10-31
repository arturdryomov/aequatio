#include "lagrangeinterpolation.h"

namespace BuiltIn {
namespace BasicFunctions {

namespace {
	LagrangeInterpolation instance;
}

LagrangeInterpolation::LagrangeInterpolation() :
	Function("lagrange_interpolation")
{
}

Rpn::Operand LagrangeInterpolation::calculate(Function::FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	Q_UNUSED(calculator);

	m_point = actualArguments.at(0).value.value<Number>();
	m_points = Rpn::Vector::toTwoDimensional(actualArguments.at(1).value.value<Rpn::Vector>());

	// Ensure all points are points actually

	foreach (QList<Number> point, m_points) {
		if (point.size() != 2) {
			THROW(EWrongArgument(QObject::tr("elements of points-list"), QObject::tr("two-dimensional points")));
		}
	}

	return Rpn::Operand(Rpn::OperandNumber, QVariant::fromValue(findSolution()));
}

QList<Rpn::Argument> LagrangeInterpolation::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandVector);

	return arguments;
}

Rpn::OperandType LagrangeInterpolation::returnValueType()
{
	return Rpn::OperandNumber;
}

Number LagrangeInterpolation::findSolution()
{
	Number result = 0;

	for (int i = 0; i < m_points.size(); i++) {
		Number basisPolynomial = 1;

		for (int j = 0; j < m_points.size(); j++) {
			if (i == j) {
				continue;
			}

			// At position "0" is "x" coordinate
			basisPolynomial *= (m_point - m_points.at(j).at(0)) / (m_points.at(i).at(0) - m_points.at(j).at(0));
		}

		// At position "1" is "y" coordinate
		result += m_points.at(i).at(1) * basisPolynomial;
	}

	return result;
}

} // namespace
} // namespace
