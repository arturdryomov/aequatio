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

	bool containsFunction(const QString &name);
	bool containsConstant(const QString &name);

	// This methods assume that corresponding functions or constants are
	// declared, otherwise an exception will be thrown.
	QList<RpnArgument> functionArguments(const QString &name);
	RpnFunction function(const QString &name);
	Number constant(const QString &name);
	QString prettyPrintedFunction(const QString &name);
	QString prettyPrintedConstant(const QString &name);

	QStringList prettyPrintedFunctions();
	QStringList prettyPrintedConstants();

	// If the constant/function cannot be added (trying to overwrite something built-in,
	// for instance), an EBuildInRedifinition or ERecursiveFunction exception will be raised.
	void addConstant(const QString &name, const Number &value);
	void addFunction(const QString &name, const RpnFunction &function);
signals:
	void constantsChanged();
	void functionsChanged();
};

#endif // DOCUMENT_H
