#ifndef GAUSS_SEIDEL_H
#define GAUSS_SEIDEL_H

#include "../function.h"

namespace BuiltIn {
namespace EquationSolving {

class GaussSeidel : public Function
{
public:
	GaussSeidel();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();

private:
	FunctionCalculator* m_calculator;

	QList<Number> findSolution(QList<QList<Number> > coefficients, Number precision);
	bool isConvergenceReached(QList<Number> previousSolution, QList<Number> currentSolution, Number precision);
	QList<QList<Number> > extractWorkingMatrix(QList<QList<Number> > coefficients);
	QList<Number> extractFreeCoefficients(QList<QList<Number> > coefficients);
	QList<Number> emptyVector(int size);
	bool hasSolution(QList<QList<Number> > matrix);

	class ENoSolution : public ECalculating {};
};

} // namespace
} // namespace

#endif // GAUSS_SEIDEL_H
