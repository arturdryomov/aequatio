#ifndef LU_SOLVING_H
#define LU_SOLVING_H

#include "../builtinfunction.h"

namespace BuiltInFunctions {
namespace EquationSolving {

class LuSolving : public Function
{
public:
	LuSolving() : Function("sle_lu")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
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
