#ifndef PARSINGEXCEPTIONS_H
#define PARSINGEXCEPTIONS_H

/* This module contains all exceptions that can be thrown by lexical or syntax analyzer.
	They are children either of EInternal or EParsing. */

#include "exceptions.h"


/* EParsing and its children */

class EParsing : public Exception
{
public:
	QString message();
};

class ELexemeExpected : public EParsing
{
public:
	ELexemeExpected(const QString &whatExpected);
	QString message();
	LogItem logItem();
private:
	QString m_expected;
};

class EEmptyInput : public EParsing
{
public:
	QString message();
};

class EUndeclaredUsed : public EParsing
{
public:
	enum ElementType {Constant, Function};
	EUndeclaredUsed(const QString &elementName, ElementType elementType);
	QString message();
private:
	QString m_elementName;
	ElementType m_elementType;
};

class EWrongArgumentsCount : public EParsing
{
public:
	EWrongArgumentsCount(const QString &functionName, int argumentsExpected);
	QString message();
private:
	QString m_functionName;
	int m_argumentsExpected;
};

class EFormalArgumentReused : public EParsing
{
public:
	EFormalArgumentReused(const QString &argumentName);
	QString message();
private:
	QString m_argumentName;
};

class EIncorrectCharacter : public EParsing
{
public:
	EIncorrectCharacter(QChar c);
	QString message();
private:
	QChar m_character;
};

// means that the function passed as an argument has incorrect signature.
class EIncorrectFunctionArgument : public EParsing
{
public:
	EIncorrectFunctionArgument(const QString &functionName);
	QString message();
private:
	QString m_functionName;
};

/* EInternal children */

class EUnsupportedLexeme : public EInternal
{
public:
	EUnsupportedLexeme(const QString &unsupportedType);
	// User does't need to know what is unsupported here.
	// This info is for debug purpose only.
protected:
	LogItem logItem();
private:
	QString m_unsupported;
};

#endif // PARSINGEXCEPTIONS_H
