#ifndef MATRICES_H
#define MATRICES_H

#include "builtinfunction.h"

namespace BuiltInFunctions {
namespace MatrixFunctions{

class MatrixSum : public Function
{
public:
	MatrixSum() : Function("matrix_sum") {}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

class MatrixDiff : public Function
{
public:
	MatrixDiff() : Function("matrix_diff") {}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

class MatrixTrace : public Function
{
public:
	MatrixTrace() : Function("matrix_trace") {}
	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

class MatrixMultiply : public Function
{
public:
	MatrixMultiply() : Function("matrix_multiply") {}
	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

class MatrixNormM : public Function
{
public:
	MatrixNormM() : Function("matrix_norm_m")	{}
	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

class MatrixNormN : public Function
{
public:
	MatrixNormN() : Function("matrix_norm_n")	{}
	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

class MatrixNormFrobenius : public Function
{
public:
	MatrixNormFrobenius() : Function("matrix_norm_frobenius") {}
	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

}
}

#endif // MATRICES_H
