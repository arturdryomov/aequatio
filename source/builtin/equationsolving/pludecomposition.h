#ifndef PLU_DECOMPOSITION_H
#define PLU_DECOMPOSITION_H

#include "../function.h"

namespace BuiltIn {
namespace EquationSolving {

class PluDecompozition : public Function
{
public:
	PluDecompozition();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
private:
	QList<QList<QList<Number> > > decompose(QList<QList<Number> > matrix);
	QList<QList<Number> > emptyMatrix(int size);

	class ENoSolution : public ECalculating {};
};

} // namespace
} // namespace

#endif // PLU_DECOMPOSITION_H
