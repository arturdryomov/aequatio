#include "incorrectinputexceptions.h"

// EIncorrectInput class methods

QString EIncorrectInput::message()
{
	return tr("Cannot parse the command entered because it is incorrect.");
}


// EExpected class methods

EExpected::EExpected(const QString &whatExpected) : m_expected(whatExpected)
{
}

QString EExpected::message()
{
	return tr("%1 expected.").arg(m_expected);
}

LogItem EExpected::logItem()
{
	LogItem item = EIncorrectInput::logItem();
	Item expectedLexeme = {"Expected lexeme", m_expected};
	item.addons << expectedLexeme;
	return item;
}


// EEmptyInput class methods

QString EEmptyInput::message()
{
	return tr("You’ve entered empty command.");
}


// EUndeclaredUsed class methods

EUndeclaredUsed::EUndeclaredUsed(const QString &elementName, ElementType elementType) :
	m_elementName(elementName),
	m_elementType(elementType)
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

