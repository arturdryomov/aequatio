#include "function.h"

namespace BuiltIn
{

QString Function::ECalculating::message()
{
	// ECalculating should be caught locally.
	Q_ASSERT_X(false, "BuiltIn::Function", "BuiltIn::Function::ECalculating::message() called.");

	return QString("");
}

EWrongArgumentsCount::EWrongArgumentsCount(const QString &parameterName, int parametersExpected) :
	m_parameterName(parameterName),
	m_parametersExpected(parametersExpected)
{
}

QString EWrongArgumentsCount::message()
{
	return tr("%1 expected %n argument(s).", "", m_parametersExpected)
		.arg(m_parameterName);
}


EWrongArgument::EWrongArgument(const QString &argumentName, const QString &argumentCondition) :
	m_argumentName(argumentName),
	m_argumentCondition(argumentCondition)
{
}

QString EWrongArgument::message()
{
	return tr("Wrong argument: %1 must be %2.").arg(m_argumentName).arg(m_argumentCondition);
}


}
