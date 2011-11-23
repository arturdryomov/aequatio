#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "logger.h"

#include <QString>
#include <QStack>

// Base exception class
class Exception
{
public:
	Exception();
	virtual QString message() = 0; // user-friendly exception message
	QStack<QString> throwStack;
	void toLogger();

protected:
	virtual LogItem logItem();
	// Helper method. tr() is simply shorter than QApplication::translate()
	QString tr(const char *sourceText, const char *disambiguation = 0, int n = -1);

private:
	QDateTime m_throwTime;
};


// Base class for exceptions that occur due to wrong algorithm. The program should be
// corrected and recompiled if such exceptions occur.
class EInternal : public Exception
{
public:
	QString message();
};


// Pushing method name to Exception::throwStack
// GCC for some reason won't compile this: E &operator <<(E &e, const QString &methodInfo)
template <class E>
E operator <<(E e, const QString &methodInfo)
{
	e.throwStack.push(methodInfo);
	return e;
}


// Exceptions throwing macros
#define THROW(e) \
	throw e << Q_FUNC_INFO;

#define RETHROW(e) \
	e << Q_FUNC_INFO; \
	throw;

#endif // EXCEPTIONS_H
