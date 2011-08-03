#include "calculatingexceptions.h"

QString ECalculating::message()
{
	return tr("An error occurred while performing calculation.");
}

EBuiltInRedifinition::EBuiltInRedifinition(const QString &elementName, EBuiltInRedifinition::ElementType elementType) :
	m_elementName(elementName), m_elementType(elementType)
{
}

QString EBuiltInRedifinition::message()
{
	if (m_elementType == Constant) {
		return tr("You cannot declare constant “%1” because there is already "
			"built-in constant with this name.").arg(m_elementName);
	}
	else {
		return tr("You cannot declare function “%1()” because there is already "
			"built-in function with this name.").arg(m_elementName);
	}
}
