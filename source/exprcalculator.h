#ifndef EXPRCALCULATOR_H
#define EXPRCALCULATOR_H

#include "rpncode.h"
#include "document.h"
#include "builtin/function.h"

#include <QHash>
#include <QObject>
#include <QMetaType>
#include <QList>

class ExprCalculator : public QObject
{
	Q_OBJECT

public:
	explicit ExprCalculator(QObject *parent = 0);
	void setDocument(Document *document);
	Rpn::Operand calculate(const Rpn::CodeThread &thread);
private:
	bool isFunction(const QString &name);	bool isConstant(const QString &name);	QList<Rpn::Argument> functionArguments(const QString &name);
	friend class FunctionCalculator;
	class FunctionCalculator : public BuiltIn::Function::FunctionCalculator
	{
	public:
		FunctionCalculator(ExprCalculator *exprCalculator) : m_exprCalculator(exprCalculator) {}
		Rpn::Operand calculate(QString functionName, QList<Rpn::Operand> actualArguments);
		QList<Rpn::Argument> functionArguments(const QString &name);
	private:
		ExprCalculator *m_exprCalculator;
	};

	FunctionCalculator *m_functionCalculator;
	Document *m_document;

	// defines whether functionName is built-in or user-defined and calculates it.
	Rpn::Operand calculateFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments);
	Rpn::Operand calculateUserDefinedFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments);
	Rpn::Operand calculateBuiltInFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments);
};

#endif // EXPRCALCULATOR_H
