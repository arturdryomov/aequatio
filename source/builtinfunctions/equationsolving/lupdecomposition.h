#ifndef LUP_DECOMPOSITION_H
#define LUP_DECOMPOSITION_H

#include "../builtinfunction.h"

namespace BuiltInFunctions {
namespace EquationSolving {

class LupDecompozition : public Function
{
public:
	LupDecompozition() : Function("lup")
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

#endif // LUP_DECOMPOSITION_H
