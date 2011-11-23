#ifndef MATRICES_H
#define MATRICES_H

#include "function.h"

namespace BuiltIn {
namespace MatrixFunctions {

class MatrixDeterminant : public Function
{
public:
	MatrixDeterminant();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
};


class MatrixSum : public Function
{
public:
	MatrixSum();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
};


class MatrixDiff : public Function
{
public:
	MatrixDiff();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
};


class MatrixTrace : public Function
{
public:
	MatrixTrace();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
};


class MatrixMultiply : public Function
{
public:
	MatrixMultiply();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
};


class MatrixNormM : public Function
{
public:
	MatrixNormM();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
};


class MatrixNormL : public Function
{
public:
	MatrixNormL();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
};


class MatrixNormFrobenius : public Function
{
public:
	MatrixNormFrobenius();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
};


class MatrixTranspose : public Function
{
public:
	MatrixTranspose();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
};


class MatrixInverse : public Function
{
public:
	MatrixInverse();

	Rpn::Operand calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments);
	QList<Rpn::Argument> requiredArguments();
	Rpn::OperandType returnValueType();
private:
	QList<QList<Number> > inverse(QList<QList<Number> > &initial);
	void swapColumns(QList<QList<Number> > &matrix, int index1, int index2);
	QList<QList <Number> > unitaryMatrix(int size);
	class ENoSolution : public ECalculating {};
};

} // namespace
} // namespace

#endif // MATRICES_H
