#ifndef GAUSS_H
#define GAUSS_H

#include "../function.h"

namespace BuiltIn {
namespace EquationSolving {

class Gauss : public Function
{
public:
	Gauss() : Function("gauss")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
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
