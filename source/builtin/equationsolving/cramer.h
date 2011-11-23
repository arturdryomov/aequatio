#ifndef CRAMER_H
#define CRAMER_H

#include "../function.h"

namespace BuiltIn {
namespace EquationSolving {

class Cramer : public Function
{
public:
	Cramer();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();

private:
	class ENoSolution : public ECalculating {};

	QList<QList<Number> > m_matrixCoefficients;
	QList<Number> findSolution();
};

} // namespace
} // namespace

#endif // CRAMER_H
