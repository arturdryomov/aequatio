#ifndef GAUSS_SEIDEL_H
#define GAUSS_SEIDEL_H

#include "../builtinfunction.h"

namespace BuiltInFunctions {
namespace EquationSolving {

class GaussSeidel : public Function
{
public:
	GaussSeidel() : Function("sle_gauss_seidel")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	FunctionCalculator* m_calculator;

	QList<Number> findSolution(QList<QList<Number> > coefficients, Number precision);
	bool isConvergenceReached(QList<Number> previousSolution, QList<Number> currentSolution, Number precision);
	QList<QList<Number> > extractWorkingMatrix(QList<QList<Number> > coefficients);
	QList<Number> extractFreeCoefficients(QList<QList<Number> > coefficients);
	QList<Number> emptyVector(int size);
};

} // namespace
} // namespace

#endif // GAUSS_SEIDEL_H
