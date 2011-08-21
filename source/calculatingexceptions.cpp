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

ERecursiveFunction::ERecursiveFunction(const QString &functionName) : m_functionName(functionName)
{
}

QString ERecursiveFunction::message()
{
	return tr("You cannot declare function “%1” this way because its calculation then will lead "
		"to infinite recursive calls.", "ERecursiveFunction").arg(m_functionName);
}


EWrongParametersCount::EWrongParametersCount(const QString &parameterName, int parametersExpected) :
m_parameterName(parameterName),
m_parametersExpected(parametersExpected)
{
}

QString EWrongParametersCount::message()
{
	return tr("%1 expected %n argument(s).", "", m_parametersExpected)
		.arg(m_parameterName);
}

EWrongArgument::EWrongArgument(const QString &argumentName, const QString &argumentCondition) :
	m_argumentName(argumentName), m_argumentCondition(argumentCondition)
{
}

QString EWrongArgument::message()
{
	return tr("Wrong argument condition: %1 must be %2.").arg(m_argumentName).arg(m_argumentCondition);
}

EWrongVectorDimension::EWrongVectorDimension(int expectedDimension, int actualDimension) :
	m_expectedDimension(expectedDimension), m_actualDimension(actualDimension)
{
}

QString EWrongVectorDimension::message()
{
		return tr("%1-dimensional vector was expected but %2-dimensional was passed.")
			.arg(m_expectedDimension)
			.arg(m_actualDimension);
}
