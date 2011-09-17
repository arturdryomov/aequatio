#include "prettyprinter.h"

PrettyPrinter::PrettyPrinter(QObject *parent) :
	QObject(parent)
{
}

QString PrettyPrinter::process(const RpnCodeThread &codeThread)
{
	Q_UNUSED(codeThread)
	return QString();
}

QString PrettyPrinter::process(const RpnFunction &function)
{
	Q_UNUSED(function)
	return QString();
}
