#ifndef LU_SOLVING_H
#define LU_SOLVING_H

#include "../function.h"

namespace BuiltIn {
namespace EquationSolving {

class LuSolving : public Function
{
public:
	LuSolving();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
private:
	FunctionCalculator* m_calculator;

	QList<Number> findSolution(QList<QList<Number> > coefficients);
	QList<QList<Number> > extractWorkingMatrix(QList<QList<Number> > coefficients);
	QList<Number> extractFreeCoefficients(QList<QList<Number> > coefficients);
	QList<QList<QList<Number> > > decompose(QList<QList<Number> > matrix);
	QList<Number> emptyVector(int size);
};

} // namespace
} // namespace

#endif // LU_SOLVING_H
