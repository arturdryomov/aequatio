#ifndef FUNCTION_H
#define FUNCTION_H

#include "../rpncode.h"
#include "../calculatingexceptions.h"
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
		// make sure 2 same-named functions are not added to the list
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

private:
	static QHash<QString, Function*> &functionsWritable()
	{
		static QHash<QString, Function*> functionsHash;
		return functionsHash;
	}
};

} // namespace

#endif // FUNCTION_H
