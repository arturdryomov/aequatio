#ifndef MATRICES_H
#define MATRICES_H

#include "builtinfunction.h"

namespace BuiltInFunctions {
namespace MatrixFunctions{

class MatrixDeterminant : public Function
{
public:
	MatrixDeterminant() : Function("matrix_det") {}

	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

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


class MatrixNormL : public Function
{
public:
	MatrixNormL() : Function("matrix_norm_l")	{}
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


class MatrixTranspose : public Function
{
public:
	MatrixTranspose() : Function("matrix_transpose") {}
	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
};

class MatrixInverse : public Function
{
public:
	MatrixInverse() : Function("matrix_inverse") {}
	RpnOperand calculate(FunctionCalculator *calculator, QList<RpnOperand> actualArguments);
	QList<RpnArgument> requiredArguments();
private:
	QList<QList<Number> > inverse(QList<QList<Number> > &initial);
	void swapColumns(QList<QList<Number> > &matrix, int index1, int index2);
	QList<QList <Number> > unitaryMatrix(int size);
	class ENoSolution : public ECalculating {};
};

} // namespace
} // namespace

#endif // MATRICES_H
