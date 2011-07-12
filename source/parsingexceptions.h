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
	QString m_elementName;
	ElementType m_elementType;
};

class EWrongArgumentsCount : public EParsing
{
public:
	EWrongArgumentsCount(const QString &functionName, int argumentsExpected, int argumentsPassed);
	QString message();
	QString m_functionName;
	int m_argumentsExpected;
	int m_argumentsPassed;
};

class EFormalArgumentReused : public EParsing
{
public:
	EFormalArgumentReused(const QString &argumentName);
	QString message();
	QString m_argumentName;
};

/* EInternal children */

class EUnsupportedLexeme : public EInternal
{
public:
	EUnsupportedLexeme(const QString &unsupportedType);
	// User does't need to know what is unsupported here.
	// This info is for debug purpose only.
private:
	QString m_unsupported;
};

class EConversionToNumber : public EInternal
{
public:
	EConversionToNumber(const QString &numberRepresentation);
	QString message();
	QString m_numberRepresentation;
};

#endif // PARSINGEXCEPTIONS_H
