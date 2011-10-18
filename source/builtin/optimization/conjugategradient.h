#ifndef CONJUGATEGRADIENT_H
#define CONJUGATEGRADIENT_H

#include "../function.h"

namespace BuiltIn {
namespace Optimization {

class ConjugateGradient : public Function
{
public:
	ConjugateGradient();

	Rpn::Operand calculate(FunctionCalculator* calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();

private:
	FunctionCalculator *m_calculator;
	QString m_functionName;
	Number m_accuracy;
	QList<Number> m_initialPoint;

	QList<Number> findMinimum();
	QList<Number> searchWithDirections(const QList<QList<Number> > directions,
		const QList<Number> &initialPoint, QList<Number> &finishingPoint, bool &found);
	Number findStep(QList<Number> point, QList<Number> direction);
	QList<QList<Number> > initialDirections(int dimensionsCount);
	bool isDirectionsLinearlyIndependend(const QList<QList<Number> > &directions);
	Number countFunction(const QList<Number> arguments);
};

} // namespace
} // namespace

#endif // CONJUGATEGRADIENT_H
