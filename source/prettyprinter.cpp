#include "prettyprinter.h"
#include "builtin/function.h"

PrettyPrinter::PrettyPrinter() : m_document(0)
{
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
	Q_ASSERT(m_document != 0);

	// This code is quite alike with Calculator::calculateFunction argorithm,
	// but we store parts of final string expression in the stack.
	// We also need to store the priority level of the last (meaning it is
	// applied after all other) part in order to determine when the braces are needed.

	QStack<PartInfo> codeParts;

	foreach(Rpn::Element element, codeThread) {

		PartInfo part;
		switch (element.type) {

			case Rpn::ElementOperand:
				part.text = element.value.value<Rpn::Operand>().toString();
				part.priority = PriorityHighest;
				break;

			case Rpn::ElementConstant:
				part.text = element.value.value<QString>();
				part.priority = PriorityHighest;
				break;

			case Rpn::ElementArgument: {
				part.text = element.value.value<QString>();
				part.priority = PriorityHighest;
				break;
			}

			case Rpn::ElementFunctionCall: {
				QString functionName = element.value.value<QString>();

				// basic arithmetical operations

				if (functionName == Rpn::FunctionPlus) {
					part.priority = PriorityPlusMinus;
					PartInfo right = codeParts.pop();
					right.bracesIfGreater(part.priority);
					PartInfo left = codeParts.pop();
					left.bracesIfGreater(part.priority);
					part.text = QString("%1 + %2").arg(left.text, right.text);
				}

				else if (functionName == Rpn::FunctionMinus) {
					part.priority = PriorityPlusMinus;
					PartInfo right = codeParts.pop();
					right.bracesIfGreaterOrEqual(part.priority);
					PartInfo left = codeParts.pop();
					left.bracesIfGreater(part.priority);
					part.text = QString("%1 − %2").arg(left.text, right.text);
				}

				else if (functionName == Rpn::FunctionMultiply) {
					part.priority = PriorityMultiplyDivide;
					PartInfo right = codeParts.pop();
					right.bracesIfGreater(part.priority);
					PartInfo left = codeParts.pop();
					left.bracesIfGreater(part.priority);
					part.text = QString("%1 × %2").arg(left.text, right.text);
				}

				else if (functionName == Rpn::FunctionDivide) {
					part.priority = PriorityMultiplyDivide;
					PartInfo right = codeParts.pop();
					right.bracesIfGreaterOrEqual(part.priority);
					PartInfo left = codeParts.pop();
					left.bracesIfGreater(part.priority);
					part.text = QString("%1 ÷ %2").arg(left.text, right.text);
				}

				else if (functionName == Rpn::FunctionPower) {
					part.priority = PriorityPower;
					PartInfo right = codeParts.pop();
					right.bracesIfGreater(part.priority);
					PartInfo left = codeParts.pop();
					left.bracesIfGreaterOrEqual(part.priority);
					part.text = QString("%1 ^ %2").arg(left.text, right.text);
				}

				else if (functionName == Rpn::FunctionUnaryMinus) {
					part.priority = PriorityPlusMinus;
					PartInfo argument = codeParts.pop();
					argument.bracesIfGreaterOrEqual(part.priority);
					part.text = QString("−%1").arg(argument.text);
				}

				// built-in and user-defined functions

				else {
					int argumentsCount;
					if (BuiltIn::Function::functions().contains(functionName)) {
						argumentsCount = BuiltIn::Function::functions()
							.value(functionName)->requiredArguments().count();
					}
					else if (m_document->containsFunction(functionName)) {
						argumentsCount = m_document->function(functionName).arguments.count();
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
