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


EUnsupportedLexeme::EUnsupportedLexeme(const QString &unsupportedType) : m_unsupported(unsupportedType)
{
}
