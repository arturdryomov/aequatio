#ifndef NELDERMEAD_H
#define NELDERMEAD_H

#include "../builtinfunction.h"

namespace BuiltInFunctions {
namespace Optimization {

// Nelder-Mead method, “метод деформируемого многогранника” in Russian terminology.

class NelderMead : public BuiltInFunction
{
public:
	NelderMead() : BuiltInFunction("nelder_mead") {}
	QList<RpnArgument> requiredArguments();
	RpnOperand calculate(FunctionCalculator* calculator, QList<RpnOperand> actualArguments);
private:
	FunctionCalculator* m_calculator;
	QString m_functionName;

	QList<Number> findMinimum(const QList<QList<Number> > &initialSimplex, Number reflectionCoefficient, Number contractionCoefficient,
	Number expansionCoefficient, Number accuracy);
	Number function(const QList<Number> &arguments);
	int indexOfMinimal(const QList<Number> &values);
	int indexOfMaximal(const QList<Number> &values);
	int indexOfSecondMaximal(const QList<Number> &values);
	QList<Number> findCenter(const QList<QList<Number> > &simplex, int worstIndex);
	bool found(const QList<QList<Number> > &simplex, const QList<Number> &center, Number accuracy);
	bool isVectorCorrect(const QList<QList<Number> > vector);
};

} // namespace
} // namespace

#endif // NELDERMEAD_H
