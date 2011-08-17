#include "parsingexceptions.h"

QString EParsing::message()
{
	return tr("Cannot parse the command entered because it is incorrect.");
}


ELexemeExpected::ELexemeExpected(const QString &whatExpected) : m_expected(whatExpected)
{
}

QString ELexemeExpected::message()
{
	return tr("%1 expected.").arg(m_expected);
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
	return tr("You’ve entered empty command.");
}

EUndeclaredUsed::EUndeclaredUsed(const QString &elementName, ElementType elementType) :
	m_elementName(elementName), m_elementType(elementType)
{
}

QString EUndeclaredUsed::message()
{
	if (m_elementType == Constant) {
		return tr("Constant “%1” was not declared by you and there is no built-in constant "
			"with this name.").arg(m_elementName);
	}
	else {
		return tr("Function “%1” was not declared by you and there is no built-in function "
			"with this name.").arg(m_elementName);
	}
}

EWrongArgumentsCount::EWrongArgumentsCount(const QString &functionName, int argumentsExpected) :
m_functionName(functionName),
m_argumentsExpected(argumentsExpected)
{
}

QString EWrongArgumentsCount::message()
{
	return tr("Function “%1” expected %n argument(s).", "", m_argumentsExpected)
		.arg(m_functionName);
}

EFormalArgumentReused::EFormalArgumentReused(const QString &argumentName) : m_argumentName(argumentName)
{
}

QString EFormalArgumentReused::message()
{
	return tr("There is already argument named “%1” in the arguments list.")
		.arg(m_argumentName);
}

EIncorrectCharacter::EIncorrectCharacter(QChar c) : m_character(c)
{
}

QString EIncorrectCharacter::message()
{
	return tr("Character “%1” is incorrect here.").arg(m_character);
}

EIncorrectConstantDeclaration::EIncorrectConstantDeclaration()
{
}

QString EIncorrectConstantDeclaration::message()
{
	return tr("Constant value can be a number or an expression the result of which is a number.");
}

EIncorrectFunctionArgument::EIncorrectFunctionArgument(const QString &functionName) : m_functionName(functionName)
{
}

QString EIncorrectFunctionArgument::message()
{
	return tr("Function “%1” cannot be passed as an argument here as it has inappropriate "
		"signature (list of arguments and their types).").arg(m_functionName);
}

EUnsupportedLexeme::EUnsupportedLexeme(const QString &unsupportedType) : m_unsupported(unsupportedType)
{
}

LogItem EUnsupportedLexeme::logItem()
{
	LogItem item = EInternal::logItem();
	Item unsupportedInfo = {"Unsupported lexeme", m_unsupported};
	item.addons << unsupportedInfo;

	return item;
}

