#include "calculatingexceptions.h"

QString ECalculating::message()
{
	return tr("An error occurred while performing calculation.", "ECalculating");
}

EBuiltInRedifinition::EBuiltInRedifinition(const QString &elementName, EBuiltInRedifinition::ElementType elementType) :
	m_elementName(elementName), m_elementType(elementType)
{
}

QString EBuiltInRedifinition::message()
{
	if (m_elementType == Constant) {
		return tr("You cannot declare constant “%1” because there is already "
			"built-in constant with this name.", "EBuiltInRedifinition").arg(m_elementName);
	}
	else {
		return tr("You cannot declare function “%1()” because there is already "
			"built-in function with this name.", "EBuiltInRedifinition").arg(m_elementName);
	}
}

ERecursiveFunction::ERecursiveFunction(const QString &functionName) : m_functionName(functionName)
{
}

QString ERecursiveFunction::message()
{
	return tr("You cannot declare function “%1” this way because its calculation then will lead "
		"to infinite recursive calls.", "ERecursiveFunction").arg(m_functionName);
}
