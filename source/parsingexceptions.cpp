#include "parsingexceptions.h"

QString EParsing::message()
{
	return tr("Cannot parse the command entered because it is incorrect.", "EParsing");
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

EUndeclaredUsed::EUndeclaredUsed(const QString &elementName, ElementType elementType) :
	m_elementName(elementName), m_elementType(elementType)
{
}

QString EUndeclaredUsed::message()
{
	if (m_elementType == Constant) {
		return tr("Constant “%1” was not declared by you and there is no built-in constant "
			"with this name.", "EUndeclaredUsed").arg(m_elementName);
	}
	else {
		return tr("Function “%1” was not declared by you and there is no built-in function "
			"with this name.", "EUndeclaredUsed").arg(m_elementName);
	}
}

EWrongArgumentsCount::EWrongArgumentsCount(const QString &functionName, int argumentsExpected, int argumentsPassed) :
	m_functionName(functionName),
	m_argumentsExpected(argumentsExpected),
	m_argumentsPassed(argumentsPassed)
{
}

QString EWrongArgumentsCount::message()
{
	return tr("Function “%1” expected %2 argument(s) but you’ve passed %3.", "EWrongArgumentsCount")
		.arg(m_functionName)
		.arg(m_argumentsExpected)
		.arg(m_argumentsPassed);
}

EFormalArgumentReused::EFormalArgumentReused(const QString &argumentName) : m_argumentName(argumentName)
{
}

QString EFormalArgumentReused::message()
{
	return tr("There is already argument named “%1” in the arguments list.", "EFormalArgumentReused")
		.arg(m_argumentName);
}

EIncorrectCharacter::EIncorrectCharacter(QChar c) : m_character(c)
{
}

QString EIncorrectCharacter::message()
{
	return tr("Character “%1” is incorrect here.", "EIncorrectCharacter").arg(m_character);
}

EUnsupportedLexeme::EUnsupportedLexeme(const QString &unsupportedType) : m_unsupported(unsupportedType)
{
}

EConversionToNumber::EConversionToNumber(const QString &numberRepresentation) :
	m_numberRepresentation(numberRepresentation)
{
}

QString EConversionToNumber::message()
{
	return tr("Error occurred while trying to convert “%1” into a number format. Let the developers "
		"know about this, please.", "EConversionToNumber").arg(m_numberRepresentation);
}
