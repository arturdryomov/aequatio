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
	QList<QList<Number> > decompose(QList<QList<Number> > matrix);
};

} // namespace
} // namespace

#endif // LU_DECOMPOSITION_H
