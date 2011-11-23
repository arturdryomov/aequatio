#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "rpncode.h"
#include "incorrectinputexceptions.h"

#include <QObject>
#include <QStringList>

class Document : public QObject
{
	Q_OBJECT

public:
	explicit Document(QObject *parent = 0);

	bool containsFunction(const QString &name) const;
	bool containsConstant(const QString &name) const;

	// This methods assume that corresponding functions or constants are
	// declared, otherwise an exception will be thrown.
	QList<Rpn::Argument> functionArguments(const QString &name) const;
	Rpn::Function function(const QString &name) const;
	Rpn::Operand constant(const QString &name) const;
	QString prettyPrintedFunction(const QString &name) const;
	QString prettyPrintedConstant(const QString &name) const;

	QStringList prettyPrintedFunctions() const;
	QStringList prettyPrintedConstants() const;

	// If the constant/function cannot be added (trying to overwrite something built-in,
	// for instance), an EBuildInRedifinition or ERecursiveFunction exception will be raised.
	void addConstant(const QString &name, const Rpn::Operand &value);
	void addFunction(const QString &name, const Rpn::Function &function);

signals:
	void constantsChanged();
	void functionsChanged();

private:
	QMap<QString, Rpn::Function> m_functions;
	QMap<QString, Rpn::Operand> m_constants;

	bool isFunctionUsed(const QString &name, const Rpn::CodeThread &codeThread) const;
};


class EBuiltInRedifinition : public EIncorrectInput
{
public:
	enum ElementType {Constant, Function};
	EBuiltInRedifinition(const QString &elementName, ElementType elementType);
	QString message();
private:
	QString m_elementName;
	ElementType m_elementType;
};


class ERecursiveFunction : public EIncorrectInput
{
public:
	ERecursiveFunction(const QString &functionName);
	QString message();
private:
	QString m_functionName;
};


#endif // DOCUMENT_H
