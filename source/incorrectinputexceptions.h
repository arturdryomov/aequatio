#ifndef INCORRECTINPUTEXCEPTIONS_H
#define INCORRECTINPUTEXCEPTIONS_H

#include "exceptions.h"

// EIncorrectInput and its successors are thrown if the command entered by user
// cannot be interpreted correctly.

class EIncorrectInput : public Exception
{
public:
	QString message();
};


class EExpected : public EIncorrectInput
{
public:
	EExpected(const QString &whatExpected);
	QString message();
	LogItem logItem();

private:
	QString m_expected;
};


class EEmptyInput : public EIncorrectInput
{
public:
	QString message();
};


class EUndeclaredUsed : public EIncorrectInput
{
public:
	enum ElementType {Constant, Function};
	EUndeclaredUsed(const QString &elementName, ElementType elementType);
	QString message();

private:
	QString m_elementName;
	ElementType m_elementType;
};

#endif // INCORRECTINPUTEXCEPTIONS_H
