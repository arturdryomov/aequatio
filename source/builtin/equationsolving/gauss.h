#ifndef GAUSS_H
#define GAUSS_H

#include "../function.h"

namespace BuiltIn {
namespace EquationSolving {

class Gauss : public Function
{
public:
	Gauss();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();

private:
	QList<Number> findSolution(QList<QList<Number> > coefficients);
	void swapColumns(QList<QList<Number> > &matrix, int index1, int index2);
	QList<Number> sizedVector(int size);
	bool areCoefficientsCorrect(const QList<QList<Number> > &coefficients);
	class ENoSolution : public ECalculating {};
};

} // namespace
} // namespace

#endif // GAUSS_H
