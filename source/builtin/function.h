#ifndef FUNCTION_H
#define FUNCTION_H

#include "../rpncode.h"
#include "../incorrectinputexceptions.h"
#include "mathutils.h"

#define _USE_MATH_DEFINES
#include <qmath.h>

namespace BuiltIn {

// Template and interface class for calculation sub-classes
class Function
{
public:
	// Class for separate calculation mechanism
	class FunctionCalculator
	{
	public:
		virtual Rpn::Operand calculate(QString functionName, QList<Rpn::Operand> actualArguments) = 0;
		virtual QList<Rpn::Argument> functionArguments(const QString &name) = 0;
	};

	// Common constructor for registration of sub-classes
	Function(QString functionName)
	{
		// Make sure 2 same-named functions are not added to the list
		Q_ASSERT(!functions().contains(functionName));

		functionsWritable().insert(functionName, this);
	}

	static const QHash<QString, Function*> &functions()
	{
		return functionsWritable();
	}

	// Calculating everything
	virtual Rpn::Operand calculate(FunctionCalculator* calculator, QList<Rpn::Operand> actualArguments) = 0;
	// Arguments for calling function
	virtual QList<Rpn::Argument> requiredArguments() = 0;
	// Type of function return value
	virtual Rpn::OperandType returnValueType() = 0;

protected:
	// A convenience class. Function successors can derive their own exceptions
	// form ECalculating and use them, but they should catch these exceptions,
	// not throw them away. See Gauss::findSolution() as an example.
	class ECalculating : public Exception
	{
	public:
		QString message();
	};

private:
	static QHash<QString, Function*> &functionsWritable()
	{
		static QHash<QString, Function*> functionsHash;
		return functionsHash;
	}
};


// Some builtin functions (like optimization functions) accept Rpn::Argument
// with type == Rpn::OperandFunctionName type, where info == ArbitraryArgumentsCount.
// In this case actual arguments count for function in OperandFunctionName is
// determined by other arguments of the builtin function and can be only checked
// while trying to calculate the builtin function. If argument count is incorrect
// EWrongArgumentsCount is thrown.
class EWrongArgumentsCount : public EIncorrectInput
{
public:
	EWrongArgumentsCount(const QString &parameterName, int argumentsExpected);
	QString message();
private:
	QString m_parameterName;
	int m_parametersExpected;
};


// Sometimes there are some constrains for arguments, passed to some builtin functions
// (e.g. 0 <= epsilon <= 1). If the actual argument does not satisfy these constrains
// EWrongArgument is thrown.
class EWrongArgument : public EIncorrectInput
{
public:
	EWrongArgument(const QString &argumentName, const QString &argumentCondition);
	QString message();
private:
	QString m_argumentName;
	QString m_argumentCondition;
};

} // namespace

#endif // FUNCTION_H
