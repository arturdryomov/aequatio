#include "exceptions.h"

#include <QApplication>
#include <typeinfo>

Exception::Exception() : m_throwTime(QDateTime::currentDateTime())
{
}

void Exception::toLogger()
{
	Logger::instance() << logItem();
}

LogItem Exception::logItem()
{
	LogItem item;
	item.time = m_throwTime;
	item.name = QString("Exception of type “%1” thrown").arg(typeid(*this).name());

	return item;
}

QString Exception::tr(const char *sourceText, const char *disambiguation, int n)
{
	return QObject::tr(sourceText, disambiguation, n);
}

QString EInternal::message()
{
	return tr("Internal error occurred. Please, report this to developers.");
}
