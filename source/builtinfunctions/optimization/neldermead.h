#ifndef NELDERMEAD_H
#define NELDERMEAD_H

#include "../builtinfunction.h"

// Nelder-Mead method, “метод деформируемого многогранника” in Russian terminology.

class NelderMead : public BuiltInFunction
{
public:
	NelderMead() : BuiltInFunction("nelder_mead") {}
	QList<RpnArgument> requiredArguments();
	RpnOperand calculate(FunctionCalculator* calculator, QList<RpnOperand> actualArguments);
private:
	QList<Number> findMinimum(const QList<QList<Number> > &initialSimplex, Number alpha, Number beta,
	Number gamma, Number epsilon);
	QString m_functionName;
	FunctionCalculator* m_calculator;
	Number function(const QList<Number> &arguments);
	int indexOfMinimal(const QList<Number> &values);
	int indexOfMaximal(const QList<Number> &values);
	int indexOfSecondMaximal(const QList<Number> &values);
	QList<Number> findCenter(const QList<QList<Number> > &simplex, int worstIndex);
	bool found(const QList<QList<Number> > &simplex, const QList<Number> &center, Number epsilon);
	bool isVectorCorrect(const QList<QList<Number> > vector);
};

#endif // NELDERMEAD_H
