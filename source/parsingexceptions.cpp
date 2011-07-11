#include "parsingexceptions.h"

QString EParsing::message()
{
	return tr("Cannot parse the command entered.", "EParsing");
}


ELexemeExpected::ELexemeExpected(const QString &whatExpected) : m_expected(whatExpected)
{
}

QString ELexemeExpected::message()
{
	return tr(QString("%1 expected.").arg(m_expected), "ELexemeExpected");
}

LogItem ELexemeExpected::logItem()
{
	LogItem item = EParsing::logItem();
	Item expectedLexeme = {"Expected lexeme", m_expected};
	item.addons << expectedLexeme;
	return item;
}

QString EEmptyInput::message()
{
	return tr("You’ve entered empty command.", "EEmptyInput");
}


EUnsupportedLexeme::EUnsupportedLexeme(const QString &unsupportedType) : m_unsupported(unsupportedType)
{
}
