#ifndef PLU_DECOMPOSITION_H
#define PLU_DECOMPOSITION_H

#include "../builtinfunction.h"

namespace BuiltInFunctions {
namespace EquationSolving {

class PluDecompozition : public Function
{
public:
	PluDecompozition() : Function("matrix_plu")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	QList<QList<QList<Number> > > decompose(QList<QList<Number> > matrix);
	QList<QList<Number> > emptyMatrix(int size);

	class ENoSolution : public ECalculating {};
};

} // namespace
} // namespace

#endif // PLU_DECOMPOSITION_H
