#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "rpncode.h"

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
	QList<RpnArgument> functionArguments(const QString &name) const;
	RpnFunction function(const QString &name) const;
	Number constant(const QString &name) const;
	QString prettyPrintedFunction(const QString &name) const;
	QString prettyPrintedConstant(const QString &name) const;

	QStringList prettyPrintedFunctions() const;
	QStringList prettyPrintedConstants() const;

	// If the constant/function cannot be added (trying to overwrite something built-in,
	// for instance), an EBuildInRedifinition or ERecursiveFunction exception will be raised.
	void addConstant(const QString &name, const Number &value);
	void addFunction(const QString &name, const RpnFunction &function);

signals:
	void constantsChanged();
	void functionsChanged();

private:
	QMap<QString, RpnFunction> m_functions;
	QMap<QString, Number> m_constants;

	bool isFunctionUsed(const QString &name, const RpnCodeThread &codeThread) const;
};

#endif // DOCUMENT_H
