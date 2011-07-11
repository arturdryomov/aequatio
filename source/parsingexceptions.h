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

#endif // PARSINGEXCEPTIONS_H
