#ifndef LAGRANGE_INTERPOLATION_H
#define LAGRANGE_INTERPOLATION_H

#include "function.h"

namespace BuiltIn {
namespace BasicFunctions {

class LagrangeInterpolation : public Function
{
public:
	LagrangeInterpolation();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();

private:
	Number m_point;
	QList<QList<Number> > m_points;

	Number findSolution();
};

} // namespace
} // namespace

#endif // LAGRANGE_INTERPOLATION_H
