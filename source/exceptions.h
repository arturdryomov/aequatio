#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QString>
#include <QStack>

// Base exception class
class Exception
{
public:
	Exception();
	virtual QString message() = 0; // user-friendly exception message
	QStack<QString> throwStack;
protected:
	QString tr(const char *text, const char *context);
};


// Base class for exceptions due to wrong algorithm. The program should be
// corrected and recompiled if such exceptions occur.
class EInternal : public Exception
{
public:
	QString message();
};


// Pushing method name to Exception::throwStack
template <class E>
E &operator <<(E &e, const QString &methodInfo)
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
