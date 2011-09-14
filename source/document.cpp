#include "document.h"

Document::Document(QObject *parent) : QObject(parent)
{
}

bool Document::containsFunction(const QString &name)
{
	Q_UNUSED(name)
	return false;
}

bool Document::containsConstant(const QString &name)
{
	Q_UNUSED(name)
	return false;
}

QList<RpnArgument> Document::functionArguments(const QString &name)
{
	Q_UNUSED(name)
	return QList<RpnArgument>();
}

RpnFunction Document::function(const QString &name)
{
	Q_UNUSED(name)
	return RpnFunction();
}

Number Document::constant(const QString &name)
{
	Q_UNUSED(name)
	return Number();
}

QString Document::prettyPrintedFunction(const QString &name)
{
	Q_UNUSED(name)
	return QString();
}

QString Document::prettyPrintedConstant(const QString &name)
{
	Q_UNUSED(name)
	return QString();
}

QStringList Document::prettyPrintedFunctions()
{
	return QStringList();
}

QStringList Document::prettyPrintedConstants()
{
	return QStringList();
}

void Document::addConstant(const QString &name, const Number &value)
{
	Q_UNUSED(name)
	Q_UNUSED(value)
}

void Document::addFunction(const QString &name, const RpnFunction &function)
{
	Q_UNUSED(name)
	Q_UNUSED(function)
}
