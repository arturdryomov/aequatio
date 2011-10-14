#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "rpncode.h"
#include "document.h"
#include "builtin/function.h"

#include <QHash>
#include <QObject>
#include <QMetaType>
#include <QList>

class Calculator : public QObject
{
	Q_OBJECT

public:
	Calculator(Document *document, QObject *parent = 0);
	~Calculator();

	void setDocument(Document *document);
	Rpn::Operand calculate(const Rpn::CodeThread &thread);

private:
	friend class FunctionCalculator;
	class FunctionCalculator : public BuiltIn::Function::FunctionCalculator
	{
	public:
		FunctionCalculator(Calculator *calculator) : m_calculator(calculator) {}
		Rpn::Operand calculate(QString functionName, QList<Rpn::Operand> actualArguments);
		QList<Rpn::Argument> functionArguments(const QString &name);

	private:
		Calculator *m_calculator;
	};

	Document *m_document;
	FunctionCalculator *m_functionCalculator;

	bool isFunction(const QString &name);
	bool isConstant(const QString &name);
	QList<Rpn::Argument> functionArguments(const QString &name);

	// defines whether functionName is built-in or user-defined and calculates it.
	Rpn::Operand calculateFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments);
	Rpn::Operand calculateUserDefinedFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments);
	Rpn::Operand calculateBuiltInFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments);
};

#endif // CALCULATOR_H
