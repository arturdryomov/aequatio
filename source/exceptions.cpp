#include "exceptions.h"

#include <QApplication>

Exception::Exception()
{
}

QString Exception::tr(const char *text, const char *context)
{
	return QApplication::translate(context, text);
}

QString EInternal::message()
{
	return tr("Internal error occurred. Please, report this to developers.", "EInternal");
}


