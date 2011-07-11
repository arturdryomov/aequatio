#include "exceptions.h"

#include <QApplication>

Exception::Exception()
{
}

QString Exception::tr(const QString &text, const QString &context)
{
	return QApplication::translate(context.toUtf8(), text.toUtf8());
}

QString EInternal::message()
{
	return tr("Internal error occurred. Please, report this to developers.", "EInternal");
}


