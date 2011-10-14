#ifndef CALCULATINGEXCEPTIONS_H
#define CALCULATINGEXCEPTIONS_H

/* This module contains all exceptions that can be thrown by expression calculator.
	They are children either of EInternal or ECalculating. */

#include "exceptions.h"

/* ECalculating and its children */

class ECalculating : public Exception
{
public:
	QString message();
};


class EBuiltInRedifinition : public ECalculating
{
public:
	enum ElementType {Constant, Function};
	EBuiltInRedifinition(const QString &elementName, ElementType elementType);
	QString message();

private:
	QString m_elementName;
	ElementType m_elementType;
};


class ERecursiveFunction : public ECalculating
{
public:
	ERecursiveFunction(const QString &functionName);
	QString message();

private:
	QString m_functionName;
};


class EWrongParametersCount : public ECalculating
{
public:
	EWrongParametersCount(const QString &parameterName, int argumentsExpected);
	QString message();

private:
	QString m_parameterName;
	int m_parametersExpected;
};


class EWrongArgument : public ECalculating
{
public:
	EWrongArgument(const QString &argumentName, const QString &argumentCondition);
	QString message();

private:
	QString m_argumentName;
	QString m_argumentCondition;
};


class EWrongVectorDimension : public ECalculating
{
public:
	EWrongVectorDimension(int expectedDimension, int actualDimension);
	QString message();

private:
	int m_expectedDimension;
	int m_actualDimension;
};

/* EInternal children */


class EIncorrectRpnCode : public EInternal
{
	/* Reason for all current non-due-to-user-input exceptions in calculator
	is incorrect RPN code. No need to be more concrete at this moment.
	There will be not a problem to derive from EIncorrectRpnCode to provide
	more specific info if necessary. */
};


#endif // CALCULATINGEXCEPTIONS_H
