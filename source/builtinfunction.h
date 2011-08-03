#ifndef BUILTINFUNCTION_H
#define BUILTINFUNCTION_H

#include "exprcalculator.h"

// Template and interface class for calculation sub-classes
class BuiltInFunction
{
public:
	BuiltInFunction(QString functionName)
	{
		functions().insert(functionName, this);
	}

	static QHash<QString, BuiltInFunction*> &functions()
	{
		static QHash<QString, BuiltInFunction*> functionsHash;
		return functionsHash;
	}

	virtual RpnOperand calculate(FunctionCalculator* calculator, QList<RpnOperand> actualArguments) = 0;

	// Class for separate calculation mechanism
	class FunctionCalculator
	{
	public:
		virtual RpnOperand calculate(QString functionName, QList<RpnOperand> actualArguments) = 0;
	};
};

#endif // BUILTINFUNCTION_H
