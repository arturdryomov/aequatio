#include "exprcalculator.h"
#include "calculatingexceptions.h"
#include "builtin/constant.h"

#include <QStack>
#include <QStringList>

ExprCalculator::ExprCalculator(QObject *parent) :
QObject(parent),
m_document(0),
m_functionCalculator(new FunctionCalculator(this))
{
}

void ExprCalculator::setDocument(Document *document)
{
	m_document = document;
}

Rpn::Operand ExprCalculator::calculate(const Rpn::CodeThread &thread)
{
	Q_ASSERT(m_document != 0);

	Rpn::Function function = {QList<Rpn::Argument>(), thread};
	m_document->addFunction(Rpn::FunctionMain, function);
	return calculateUserDefinedFunction(Rpn::FunctionMain, QList<Rpn::Operand>());
}

Rpn::Operand ExprCalculator::calculateFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments)
{
	if (BuiltIn::Function::functions().contains(functionName)) {
		// check for argument types equivalence
		QList<Rpn::Argument> requiredArguments = BuiltIn::Function::functions().
		value(functionName)->requiredArguments();
		for (int i = 0; i < requiredArguments.count(); ++i) {
			if (actualArguments.at(i).type != requiredArguments.at(i).type) {
				THROW(EIncorrectRpnCode());
			}
		}

		// calculate
		return calculateBuiltInFunction(functionName, actualArguments);
	}

	else if (m_document->containsFunction(functionName)) {
		// no type checks here at the moment as user-defined functions can take only
		// numbers as arguments.
		return calculateUserDefinedFunction(functionName, actualArguments);
	}

	else {
		THROW(EIncorrectRpnCode());
	}
}

Rpn::Operand ExprCalculator::calculateUserDefinedFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments)
{
	QStack<Rpn::Operand> calculationStack;

	Rpn::CodeThread currentThread = m_document->function(functionName).codeThread;

	// Find and extract

	foreach (Rpn::Element element, currentThread) {

		switch (element.type) {
			// Just push to stack
			case Rpn::ElementOperand: {
				calculationStack.push(element.value.value<Rpn::Operand>());
				break;
			}

				// Get actual value and push it to stack
			case Rpn::ElementArgument: {
				// We will not check for argument types here at his time as currently user-defined functions
				// can only take numbers as arguments.
				QString argumentName = element.value.value<QString>();
				Rpn::Function function = m_document->function(functionName);

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
			case Rpn::ElementConstant: {
				Rpn::Operand operand(Rpn::OperandNumber);
				if (BuiltIn::Constant::constants().contains(element.value.toString())) {
					operand.value = BuiltIn::Constant::constants().value(element.value.toString());
				}
				else if (m_document->containsConstant(element.value.toString())) {
					operand.value = m_document->constant(element.value.toString());
				}
				else {
					THROW(EIncorrectRpnCode());
				}
				calculationStack.push(operand);
				
				break;
			}

				// Find function and call it
			case Rpn::ElementFunctionCall: {
				QString callingFunctionName = element.value.value<QString>();

				QList<Rpn::Operand> actualArguments;
				for (int i = 0; i < functionArguments(callingFunctionName).count(); ++i) {
					actualArguments.prepend(calculationStack.pop());
				}

				Rpn::Operand result = calculateFunction(callingFunctionName, actualArguments);
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

	Rpn::Operand result = calculationStack.pop();
	// currently user-defined functions can return only numbers or vectors.
	if ((result.type != Rpn::OperandNumber) && (result.type != Rpn::OperandVector)) {
		THROW(EIncorrectRpnCode());
	}
	return result;
}

Rpn::Operand ExprCalculator::calculateBuiltInFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments)
{
	return BuiltIn::Function::functions().value(functionName)->calculate(
	m_functionCalculator, actualArguments);
}

bool ExprCalculator::isFunction(const QString &name)
{
	return (m_document->containsFunction(name) || BuiltIn::Function::functions().contains(name));
}

bool ExprCalculator::isConstant(const QString &name)
{
	return (m_document->containsConstant(name) || BuiltIn::Constant::constants().contains(name));
}

QList<Rpn::Argument> ExprCalculator::functionArguments(const QString &name)
{
	if (BuiltIn::Function::functions().contains(name)) {
		return BuiltIn::Function::functions().value(name)->requiredArguments();
	}

	else if (m_document->containsFunction(name)) {
		return m_document->function(name).arguments;
	}
	else {
		THROW(EIncorrectRpnCode());
	}
}

Rpn::Operand ExprCalculator::FunctionCalculator::calculate(QString functionName, QList<Rpn::Operand> actualArguments)
{
	return m_exprCalculator->calculateFunction(functionName, actualArguments);
}

QList<Rpn::Argument> ExprCalculator::FunctionCalculator::functionArguments(const QString &name)
{
	return m_exprCalculator->functionArguments(name);
}
