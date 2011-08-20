#ifndef CRAMER_H
#define CRAMER_H

#include "builtinfunction.h"

class Cramer : public BuiltInFunction
{
public:
	Cramer() : BuiltInFunction("cramer")
	{
	}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	QList<QList<Number> > m_coefficientsMatrix;

	QList<Number> solveEquationSystem();
};

#endif // CRAMER_H
