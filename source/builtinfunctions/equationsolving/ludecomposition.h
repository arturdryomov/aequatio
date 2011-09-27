#ifndef LU_DECOMPOSITION_H
#define LU_DECOMPOSITION_H

#include "../builtinfunction.h"

namespace BuiltInFunctions {
namespace EquationSolving {

class LuDecompozition : public Function
{
public:
	LuDecompozition() : Function("lu")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	QList<QList<QList<Number> > > decompose(const QList<QList<Number> > &matrix);
	QList<QList<Number> > emptyMatrix(int size);
};

} // namespace
} // namespace

#endif // LU_DECOMPOSITION_H
