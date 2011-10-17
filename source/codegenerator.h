#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "document.h"
#include "incorrectinputexceptions.h"

enum BinaryOperation {
	BinaryOperationPlus,
	BinaryOperationMinus,
	BinaryOperationMultiply,
	BinaryOperationDivide,
	BinaryOperationPower
};


class CodeGenerator
{
public:
	CodeGenerator(Document *document);

	void setDocument(Document *document);
	void addConstant(const QString &name, const Rpn::Operand &value);
	void addFunction(const QString &name, const QList<QString> &formalArgumentNames,
		const Rpn::CodeThread &body);
	Rpn::CodeThread generateBinaryOperation(BinaryOperation operation, Rpn::CodeThread left,
		Rpn::CodeThread right);
	Rpn::CodeThread generateUnaryMinus(Rpn::CodeThread operand);
	Rpn::CodeThread generateNumber(Number number);
	Rpn::CodeThread generateConstant(const QString &name);
	Rpn::CodeThread generateFormalArgument(const QString &name);
	Rpn::CodeThread generateFunction(const QString &name, const QList<Rpn::CodeThread> &actualArguments);
	Rpn::CodeThread generateFunctonNameOperand(const QString &name);
	Rpn::Vector generateVector(const QList<Number> &elements); // 1-dimensional
	Rpn::Vector generateVector(const QList<Rpn::Vector> &elements); // multi-dimensional
	Rpn::CodeThread packVector(const Rpn::Vector &vector);

private:
	Document *m_document;

	// Returns the type of the result of the expression in the thread.
	Rpn::OperandType codeThreadExpressionType(const Rpn::CodeThread &thread);

	Rpn::OperandType defineFunctionReturnType(const QString &functionName);
	QList<Rpn::Argument> functionFormalArguments(const QString &functionName);
	bool isFunctionSignatureSuitable(const QString &functionName, int argumentsCount);
	bool areFormalActualArgumentsCompliant(Rpn::Argument formalArgument,
		const Rpn::CodeThread &actualArgumentCodeThread);
};


class ENotNumberConstant : public EIncorrectInput
{
public:
	ENotNumberConstant();
	QString message();
};


class EIncorrectFunctionArgument : public EIncorrectInput
{
public:
	EIncorrectFunctionArgument(const QString &functionName);
	QString message();

private:
	QString m_functionName;
};


class EWrongArgumentsCount : public EIncorrectInput
{
public:
	EWrongArgumentsCount(const QString &functionName, int argumentsExpected);
	QString message();

private:
	QString m_functionName;
	int m_argumentsExpected;
};


class EWrongArgumentType : public EIncorrectInput
{
public:
	EWrongArgumentType(const QString &functionName, int argumentIndex);
	QString message();

private:
	QString m_functionName;
	int m_argumentIndex;
};

#endif // CODEGENERATOR_H
