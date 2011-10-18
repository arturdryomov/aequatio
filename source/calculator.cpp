#include "calculator.h"
#include "builtin/constant.h"

#include <QStack>
#include <QStringList>

Calculator::Calculator(Document *document, QObject *parent) :
	QObject(parent),
	m_functionCalculator(new FunctionCalculator(this))
{
	setDocument(document);
}

Calculator::~Calculator()
{
	delete m_functionCalculator;
}

void Calculator::setDocument(Document *document)
{
	if (document == 0) {
		THROW(EInternal());
	}
	m_document = document;
}

Rpn::Operand Calculator::calculate(const Rpn::CodeThread &thread)
{
	// Construct Main function, that includes child thread
	Rpn::Function function = {QList<Rpn::Argument>(), thread};
	m_document->addFunction(Rpn::FunctionMain, function);
	return calculateUserDefinedFunction(Rpn::FunctionMain, QList<Rpn::Operand>());
}

Rpn::Operand Calculator::calculateFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments)
{
	// Ensure no Rpn::OperandIncorrect passed as an argument
	foreach (const Rpn::Operand &operand, actualArguments) {
		if (operand.type == Rpn::OperandIncorrect) {
			return Rpn::Operand(Rpn::OperandIncorrect);
		}
	}

	if (BuiltIn::Function::functions().contains(functionName)) {
		// Check for argument types equivalence
		QList<Rpn::Argument> requiredArguments = BuiltIn::Function::functions()
			.value(functionName)->requiredArguments();
		for (int i = 0; i < requiredArguments.size(); ++i) {
			if (actualArguments.at(i).type != requiredArguments.at(i).type) {
				THROW(EIncorrectRpnCode());
			}
		}

		// Calculate
		return calculateBuiltInFunction(functionName, actualArguments);
	}

	else if (m_document->containsFunction(functionName)) {
		// No type checks here at the moment as user-defined functions can take only numbers as arguments
		return calculateUserDefinedFunction(functionName, actualArguments);
	}

	else {
		THROW(EIncorrectRpnCode());
	}
}

Rpn::Operand Calculator::calculateUserDefinedFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments)
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
				// can only take numbers as arguments
				QString argumentName = element.value.value<QString>();
				Rpn::Function function = m_document->function(functionName);

				for (int i = 0; i < function.arguments.size(); ++i) {
					// Find ordinal number of the argument with name argumentName
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
					operand = m_document->constant(element.value.toString());
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
				for (int i = 0; i < functionArguments(callingFunctionName).size(); ++i) {
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

	if (calculationStack.size() != 1) {
		THROW(EIncorrectRpnCode());
	}

	// Return the result -- it might be an operand of any type
	Rpn::Operand result = calculationStack.pop();
	return result;
}

Rpn::Operand Calculator::calculateBuiltInFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments)
{
	return BuiltIn::Function::functions().value(functionName)->calculate(m_functionCalculator, actualArguments);
}

bool Calculator::isFunction(const QString &name)
{
	return (m_document->containsFunction(name) || BuiltIn::Function::functions().contains(name));
}

bool Calculator::isConstant(const QString &name)
{
	return (m_document->containsConstant(name) || BuiltIn::Constant::constants().contains(name));
}

QList<Rpn::Argument> Calculator::functionArguments(const QString &name)
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

Rpn::Operand Calculator::FunctionCalculator::calculate(QString functionName, QList<Rpn::Operand> actualArguments)
{
	return m_calculator->calculateFunction(functionName, actualArguments);
}

QList<Rpn::Argument> Calculator::FunctionCalculator::functionArguments(const QString &name)
{
	return m_calculator->functionArguments(name);
}
