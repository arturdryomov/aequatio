#include "prettyprinter.h"
#include "builtin/function.h"

PrettyPrinter::PrettyPrinter(const Document *document)
{
	setDocument(document);
}

void PrettyPrinter::setDocument(const Document *document)
{
	if (document == 0) {
		THROW(EInternal());
	}
	m_document = document;
}

QString PrettyPrinter::process(const Rpn::CodeThread &codeThread)
{
	// This code is quite alike with Calculator::calculateFunction argorithm,
	// but we store parts of final string expression in the stack.
	// We also need to store the priority level of the last (meaning it is
	// applied after all other) part in order to determine when the braces are needed.

	// Clear old data from stack for safety
	m_codeParts.clear();

	foreach(Rpn::Element element, codeThread) {

		PartInfo codePart;

		switch (element.type) {

			case Rpn::ElementOperand:
				codePart.text = element.value.value<Rpn::Operand>().toString();
				codePart.priority = PriorityHighest;
				break;

			case Rpn::ElementConstant:
				codePart.text = element.value.value<QString>();
				codePart.priority = PriorityHighest;
				break;

			case Rpn::ElementArgument: {
				codePart.text = element.value.value<QString>();
				codePart.priority = PriorityHighest;
				break;
			}

			case Rpn::ElementFunctionCall: {
				QString functionName = element.value.value<QString>();
				codePart = constructFunctionPart(functionName);
				break;
			}
			default:
				THROW(EIncorrectRpnCode());
		}

		m_codeParts.push(codePart);
	}

	if (m_codeParts.size() != 1) {
		THROW(EIncorrectRpnCode());
	}

	return m_codeParts.pop().text;
}

PrettyPrinter::PartInfo PrettyPrinter::constructFunctionPart(QString functionName)
{
	PartInfo codePart;

	// Basic arithmetical operations

	if (functionName == Rpn::FunctionPlus) {
		codePart.priority = PriorityPlusMinus;
		PartInfo rightArgument = m_codeParts.pop();
		rightArgument.bracesIfGreater(codePart.priority);
		PartInfo leftArgument = m_codeParts.pop();
		leftArgument.bracesIfGreater(codePart.priority);
		codePart.text = QString("%1 + %2").arg(leftArgument.text, rightArgument.text);
	}

	else if (functionName == Rpn::FunctionMinus) {
		codePart.priority = PriorityPlusMinus;
		PartInfo rightArgument = m_codeParts.pop();
		rightArgument.bracesIfGreaterOrEqual(codePart.priority);
		PartInfo leftArgument = m_codeParts.pop();
		leftArgument.bracesIfGreater(codePart.priority);
		codePart.text = QString("%1 − %2").arg(leftArgument.text, rightArgument.text);
	}

	else if (functionName == Rpn::FunctionMultiply) {
		codePart.priority = PriorityMultiplyDivide;
		PartInfo rightArgument = m_codeParts.pop();
		rightArgument.bracesIfGreater(codePart.priority);
		PartInfo leftArgument = m_codeParts.pop();
		leftArgument.bracesIfGreater(codePart.priority);
		codePart.text = QString("%1 × %2").arg(leftArgument.text, rightArgument.text);
	}

	else if (functionName == Rpn::FunctionDivide) {
		codePart.priority = PriorityMultiplyDivide;
		PartInfo rightArgument = m_codeParts.pop();
		rightArgument.bracesIfGreaterOrEqual(codePart.priority);
		PartInfo leftArgument = m_codeParts.pop();
		leftArgument.bracesIfGreater(codePart.priority);
		codePart.text = QString("%1 ÷ %2").arg(leftArgument.text, rightArgument.text);
	}

	else if (functionName == Rpn::FunctionPower) {
		codePart.priority = PriorityPower;
		PartInfo rightArgument = m_codeParts.pop();
		rightArgument.bracesIfGreater(codePart.priority);
		PartInfo leftArgument = m_codeParts.pop();
		leftArgument.bracesIfGreaterOrEqual(codePart.priority);
		codePart.text = QString("%1 ^ %2").arg(leftArgument.text, rightArgument.text);
	}

	else if (functionName == Rpn::FunctionUnaryMinus) {
		codePart.priority = PriorityPlusMinus;
		PartInfo argument = m_codeParts.pop();
		argument.bracesIfGreaterOrEqual(codePart.priority);
		codePart.text = QString("−%1").arg(argument.text);
	}

	// Built-in and user-defined functions

	else {
		int argumentsCount;
		if (BuiltIn::Function::functions().contains(functionName)) {
			argumentsCount = BuiltIn::Function::functions()
				.value(functionName)->requiredArguments().size();
		}
		else if (m_document->containsFunction(functionName)) {
			argumentsCount = m_document->function(functionName).arguments.size();
		}
		else {
			THROW(EIncorrectRpnCode());
		}

		QStringList arguments;
		for (int i = 0; i < argumentsCount; i++) {
			arguments.prepend(m_codeParts.pop().text);
			codePart.text = QString("%1(%2)").arg(functionName).arg(arguments.join(", "));
			codePart.priority = PriorityFunction;
		}
	}

	return codePart;
}

QString PrettyPrinter::process(const Rpn::Function &function, const QString &functionName)
{
	QStringList arguments;

	foreach (const Rpn::Argument &argument, function.arguments) {
		arguments << argument.name;
	}

	return QString("%1(%2) = %3")
		.arg(functionName)
		.arg(arguments.join(", "))
		.arg(process(function.codeThread));
}
