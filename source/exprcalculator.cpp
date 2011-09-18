#include "exprcalculator.h"
#include "calculatingexceptions.h"
#include "builtin/constant.h"

#include <QStack>
#include <QStringList>

ExprCalculator::ExprCalculator(QObject *parent) :
	QObject(parent),
	m_functionCalculator(new FunctionCalculator(this))
{
}

ExpressionDescription ExprCalculator::calculate(const RpnCodeThread &thread)
{
	RpnFunction function = {QList<RpnArgument>(), thread};
	m_userDefinedFunctions.insert(RpnFunctionMain, function);
	ExpressionDescription description = {rpnCodeThreadToString(thread),
		calculateUserDefinedFunction(RpnFunctionMain, QList<RpnOperand>()) };

	return description;
}

RpnOperand ExprCalculator::calculateFunction(const QString &functionName, const QList<RpnOperand> &actualArguments)
{
	if (BuiltIn::Function::functions().contains(functionName)) {
		// check for argument types equivalence
		QList<RpnArgument> requiredArguments = BuiltIn::Function::functions().
			value(functionName)->requiredArguments();
		for (int i = 0; i < requiredArguments.count(); ++i) {
			if (actualArguments.at(i).type != requiredArguments.at(i).type) {
				THROW(EIncorrectRpnCode());
			}
		}

		// calculate
		return calculateBuiltInFunction(functionName, actualArguments);
	}

	else if (m_userDefinedFunctions.contains(functionName)) {
		// no type checks here at the moment as user-defined functions can take only
		// numbers as arguments.
		return calculateUserDefinedFunction(functionName, actualArguments);
	}

	else {
		THROW(EIncorrectRpnCode());
	}
}

RpnOperand ExprCalculator::calculateUserDefinedFunction(const QString &functionName, const QList<RpnOperand> &actualArguments)
{
	QStack<RpnOperand> calculationStack;

	RpnCodeThread currentThread = m_userDefinedFunctions.value(functionName).codeThread;

	// Find and extract

	foreach (RpnElement element, currentThread) {

		switch (element.type) {
			// Just push to stack
			case RpnElementOperand: {
				calculationStack.push(element.value.value<RpnOperand>());
				break;
			}

			// Get actual value and push it to stack
			case RpnElementArgument: {
				// We will not check for argument types here at his time as currently user-defined functions
				// can only take numbers as arguments.
				QString argumentName = element.value.value<QString>();
				RpnFunction function = m_userDefinedFunctions.value(functionName);

				for (int i = 0; i < function.arguments.count(); ++i) {
					// find ordinal number of the argument with name argumentName
					// and substitute it with actual value from funtionArguments
					if (argumentName == function.arguments.at(i).name) {
						calculationStack.push(actualArguments[i]);
						break;
					}
				}
				break;
			}

			// Find constant and push its value
			case RpnElementConstant: {
				RpnOperand operand(RpnOperandNumber);

                if (BuiltIn::Constant::constants().contains(element.value.toString())) {
                    operand.value = BuiltIn::Constant::constants().value(element.value.toString());
				}
				else if (m_userDefinedConstants.contains(element.value.toString())) {
					operand.value = m_userDefinedConstants.value(element.value.toString());
				}
				else {
					THROW(EIncorrectRpnCode());
				}
				calculationStack.push(operand);
				
				break;
			}

			// Find function and call it
			case RpnElementFunctionCall: {
				QString callingFunctionName = element.value.value<QString>();

				QList<RpnOperand> actualArguments;
				for (int i = 0; i < functionArguments(callingFunctionName).count(); ++i) {
					actualArguments.prepend(calculationStack.pop());
				}

				RpnOperand result = calculateFunction(callingFunctionName, actualArguments);
				calculationStack.push(result);

				break;
			}

			default:
				THROW(EIncorrectRpnCode());
		}
	}

	if (calculationStack.count() != 1) {
		THROW(EIncorrectRpnCode());
	}

	RpnOperand result = calculationStack.pop();
	// currently user-defined functions can return only numbers or vectors.
	if ((result.type != RpnOperandNumber) && (result.type != RpnOperandVector)) {
		THROW(EIncorrectRpnCode());
	}
	return result;
}

RpnOperand ExprCalculator::calculateBuiltInFunction(const QString &functionName, const QList<RpnOperand> &actualArguments)
{
	return BuiltIn::Function::functions().value(functionName)->calculate(
		m_functionCalculator, actualArguments);
}

FunctionDescription ExprCalculator::functionDescription(const QString &functionName)
{
	RpnFunction functionCode = m_userDefinedFunctions.value(functionName);
	FunctionDescription description;
	description.name = functionName;
	foreach (RpnArgument argument, functionCode.arguments) {
		description.arguments << argument.name;
	}
	description.body = rpnCodeThreadToString(functionCode.codeThread);
	return description;
}

QString ExprCalculator::rpnCodeThreadToString(const RpnCodeThread &codeThread)
{
	// This code is quite alike with calculateFunction argorithm, but we store
	// parts of final string expression in the stack.
	// We also need to store the priority level of the last (meaning it is applied after all other)
	// part in order to determine when the braces are needed.

	QStack<PartInfo> codeParts;

	foreach(RpnElement element, codeThread) {

		PartInfo part;
		switch (element.type) {

			case RpnElementOperand:
				part.text = element.value.value<RpnOperand>().toString();
				part.priority = PriorityHighest;
				break;

			case RpnElementConstant:
				part.text = element.value.value<QString>();
				part.priority = PriorityHighest;
				break;

			case RpnElementArgument: {
				part.text = element.value.value<QString>();
				part.priority = PriorityHighest;
				break;
			}

			case RpnElementFunctionCall: {
				QString functionName = element.value.value<QString>();

				// basic arithmetical operations
				if (functionName == RpnFunctionPlus) {
					part.priority = PriorityPlusMinus;
					PartInfo right = codeParts.pop();
					right.bracesIfGreater(part.priority);
					PartInfo left = codeParts.pop();
					left.bracesIfGreater(part.priority);
					part.text = QString("%1 + %2").arg(left.text, right.text);
				}

				else if (functionName == RpnFunctionMinus) {
					part.priority = PriorityPlusMinus;
					PartInfo right = codeParts.pop();
					right.bracesIfGreaterOrEqual(part.priority);
					PartInfo left = codeParts.pop();
					left.bracesIfGreater(part.priority);
					part.text = QString("%1 − %2").arg(left.text, right.text);
				}

				else if (functionName == RpnFunctionMultiply) {
					part.priority = PriorityMultiplyDivide;
					PartInfo right = codeParts.pop();
					right.bracesIfGreater(part.priority);
					PartInfo left = codeParts.pop();
					left.bracesIfGreater(part.priority);
					part.text = QString("%1 × %2").arg(left.text, right.text);
				}

				else if (functionName == RpnFunctionDivide) {
					part.priority = PriorityMultiplyDivide;
					PartInfo right = codeParts.pop();
					right.bracesIfGreaterOrEqual(part.priority);
					PartInfo left = codeParts.pop();
					left.bracesIfGreater(part.priority);
					part.text = QString("%1 ÷ %2").arg(left.text, right.text);
				}

				else if (functionName == RpnFunctionPower) {
					part.priority = PriorityPower;
					PartInfo right = codeParts.pop();
					right.bracesIfGreater(part.priority);
					PartInfo left = codeParts.pop();
					left.bracesIfGreaterOrEqual(part.priority);
					part.text = QString("%1 ^ %2").arg(left.text, right.text);
				}

				else if (functionName == RpnFunctionUnaryMinus) {
					part.priority = PriorityPlusMinus;
					PartInfo argument = codeParts.pop();
					argument.bracesIfGreaterOrEqual(part.priority);
					part.text = QString("−%1").arg(argument.text);
				}

				// built-in and user-defined functions
				else {
					int argumentsCount;
					if (BuiltIn::Function::functions().contains(functionName)) {
						argumentsCount = BuiltIn::Function::functions().
							value(functionName)->requiredArguments().count();
					}
					else if (m_userDefinedFunctions.contains(functionName)) {
						argumentsCount = m_userDefinedFunctions.value(functionName).arguments.count();
					}
					else {
						THROW(EIncorrectRpnCode());
					}

					QStringList arguments;
					for (int i = 0; i < argumentsCount; i++) {
						arguments.prepend(codeParts.pop().text);
						part.text = QString("%1(%2)").arg(functionName).arg(arguments.join(", "));
						part.priority = PriorityFunction;
					}
				}
				break;
			}
			default:
				THROW(EIncorrectRpnCode());
		} // switch
		codeParts.push(part);

	} // foreach

	if (codeParts.count() != 1) {
		THROW(EIncorrectRpnCode());
	}

	return codeParts.pop().text;
}

ConstantDescription ExprCalculator::addConstant(const QString &name, const Number &value)
{
    if (BuiltIn::Constant::constants().contains(name)) {
		THROW(EBuiltInRedifinition(name, EBuiltInRedifinition::Constant));
	}

	m_userDefinedConstants.insert(name, value);
	emit constantsListChanged();

	ConstantDescription constantDescription = {name, value};
	return constantDescription;
}

FunctionDescription ExprCalculator::addFunction(const QString &name, const RpnFunction &function)
{
	if (BuiltIn::Function::functions().contains(name)) {
		THROW(EBuiltInRedifinition(name, EBuiltInRedifinition::Function));
	}

	if (isFunctionUsed(name, function.codeThread)) {
		THROW(ERecursiveFunction(name));
	}

	m_userDefinedFunctions.insert(name, function);

	emit functionsListChanged();

	return functionDescription(name);
}

bool ExprCalculator::isFunction(const QString &name)
{
	return (m_userDefinedFunctions.contains(name) || BuiltIn::Function::functions().contains(name));
}

bool ExprCalculator::isConstant(const QString &name)
{
    return (m_userDefinedConstants.contains(name) || BuiltIn::Constant::constants().contains(name));
}

QList<RpnArgument> ExprCalculator::functionArguments(const QString &name)
{
	if (BuiltIn::Function::functions().contains(name)) {
		return BuiltIn::Function::functions().value(name)->requiredArguments();
	}

	else if (m_userDefinedFunctions.contains(name)) {
		return m_userDefinedFunctions.value(name).arguments;
	}
	else {
		THROW(EIncorrectRpnCode());
	}
}

QList<ConstantDescription> ExprCalculator::constantsList()
{
	QList<ConstantDescription> constantsList;

	QMapIterator<QString, Number> i(m_userDefinedConstants);
	while (i.hasNext()) {
		i.next();
		ConstantDescription constant = {i.key(), i.value()};
		constantsList << constant;
	}

	return constantsList;
}

QList<FunctionDescription> ExprCalculator::functionsList()
{
	QList<FunctionDescription> functionsList;

	QMapIterator<QString, RpnFunction> i(m_userDefinedFunctions);
	while (i.hasNext()) {
		i.next();
		if (i.key() != RpnFunctionMain) {
			FunctionDescription function = functionDescription(i.key());
			functionsList << function;
		}
	}

	return functionsList;
}

bool ExprCalculator::isFunctionUsed(const QString &functionName, const RpnCodeThread &code)
{
	foreach (RpnElement element, code) {
		if (element.type != RpnElementFunctionCall) continue;

		QString calledFunctionName = element.value.value<QString>();
		if (!m_userDefinedFunctions.contains(calledFunctionName)) continue;

		// is this our function call
		if (calledFunctionName == functionName) {
			return true;
		}

		// check recursively
		RpnCodeThread calledFunctionCode = m_userDefinedFunctions.value(calledFunctionName).codeThread;
		if (isFunctionUsed(functionName, calledFunctionCode)) {
			return true;
		}
	}

	return false;
}

RpnOperand ExprCalculator::FunctionCalculator::calculate(QString functionName, QList<RpnOperand> actualArguments)
{
	return m_exprCalculator->calculateFunction(functionName, actualArguments);
}

QList<RpnArgument> ExprCalculator::FunctionCalculator::functionArguments(const QString &name)
{
	return m_exprCalculator->functionArguments(name);
}
