#include "codegenerator.h"
#include "parsingexceptions.h"
#include "builtin/constant.h"
#include "builtin/function.h"

CodeGenerator::CodeGenerator(Document *document)
{
	setDocument(document);
}

void CodeGenerator::setDocument(Document *document)
{
	if (document == 0) {
		THROW(EInternal());
	}

	m_document = document;
}

void CodeGenerator::addConstant(const QString &name, const Rpn::Operand &value)
{
	// constant values currently can only be of Number type
	if (value.type != Rpn::OperandNumber) {
		THROW(EIncorrectConstantDeclaration());
	}

	// NOTE: Value.value.value is stupid. Should be fixed in some way.
	m_document->addConstant(name, value.value.value<Number>());
}

void CodeGenerator::addFunction(const QString &name, const QList<QString> &formalArgumentNames,
	const Rpn::CodeThread &body)
{
	Rpn::Function function;

	foreach (const QString &formalArgumentName, formalArgumentNames) {
		// user-defined functions now take only numbers as arguments
		function.arguments << Rpn::Argument(Rpn::OperandNumber, formalArgumentName);
	}
	function.codeThread = body;

	m_document->addFunction(name, function);
}

Rpn::CodeThread CodeGenerator::generateBinaryOperation(BinaryOperation operation,
	Rpn::CodeThread left, Rpn::CodeThread right)
{
	QList<Rpn::CodeThread> arguments;
	arguments << left << right;

	switch (operation) {
		case BinaryOperationPlus:		
			return generateFunction(Rpn::FunctionPlus, arguments);

		case BinaryOperationMinus:
			return generateFunction(Rpn::FunctionMinus, arguments);

		case BinaryOperationMultiply:
			return generateFunction(Rpn::FunctionMultiply, arguments);

		case BinaryOperationDivide:
			return generateFunction(Rpn::FunctionDivide, arguments);

		case BinaryOperationPower:
			return generateFunction(Rpn::FunctionPower, arguments);

		default:
			THROW(EInternal());
	}
}

Rpn::CodeThread CodeGenerator::generateUnaryMinus(Rpn::CodeThread operand)
{
	QList<Rpn::CodeThread> arguments;
	arguments << operand;

	return generateFunction(Rpn::FunctionUnaryMinus, arguments);
}

Rpn::CodeThread CodeGenerator::generateNumber(Number number)
{
	Rpn::CodeThread result;
	Rpn::Operand operand(Rpn::OperandNumber, number);
	result << Rpn::Element(Rpn::ElementOperand, QVariant::fromValue(operand));

	return result;
}

Rpn::CodeThread CodeGenerator::generateConstant(const QString &name)
{
	if (!m_document->containsConstant(name) && !BuiltIn::Constant::constants().contains(name)) {
		THROW(EUndeclaredUsed(name, EUndeclaredUsed::Constant));
	}

	Rpn::CodeThread result;	
	result << Rpn::Element(Rpn::ElementConstant, name);;

	return result;
}

Rpn::CodeThread CodeGenerator::generateFormalArgument(const QString &name)
{
	Rpn::CodeThread result;
	result << Rpn::Element(Rpn::ElementArgument, name);

	return result;
}

Rpn::CodeThread CodeGenerator::generateFunction(const QString &name, const QList<Rpn::CodeThread> &actualArguments)
{
	// determine functions formal arguments
	const QList<Rpn::Argument> &formalArguments = functionFormalArguments(name);

	// Check whether actual arguments comply with respective formal arguments

	if (actualArguments.count() != formalArguments.count()) {
		THROW(EWrongArgumentsCount(name, formalArguments.count()));
	}

	for (int i = 0; i < actualArguments.count(); ++i) {
		checkFormalActualArgumentsCompliance(formalArguments.at(i), actualArguments.at(i));
	}

	// build resulting thread
	Rpn::CodeThread result;
	for (int i = 0; i < actualArguments.count(); ++i) {
		result << actualArguments.at(i);
	}
	result << Rpn::Element(Rpn::ElementFunctionCall, name);

	return result;
}

Rpn::CodeThread CodeGenerator::generateFunctonNameOperand(const QString &name)
{
	Rpn::Operand operand(Rpn::OperandFunctionName, QVariant::fromValue(name));
	Rpn::CodeThread result;
	result << Rpn::Element(Rpn::ElementOperand, QVariant::fromValue(operand));
	return result;
}

Rpn::Vector CodeGenerator::generateVector(const QList<Number> &elements)
{
	Q_ASSERT(elements.count() > 0);

	Rpn::Vector result;
	foreach (Number element, elements) {
		result.values << QVariant::fromValue(element);
	}

	return result;
}

Rpn::Vector CodeGenerator::generateVector(const QList<Rpn::Vector> &elements)
{
	Q_ASSERT(elements.count() > 0);
	Q_ASSERT(elements.first().values.count() > 0);

	int innerDimensionsCount = elements.first().values.first().value<Rpn::Vector>().dimensions;
	Rpn::Vector result(innerDimensionsCount + 1);

	foreach (const Rpn::Vector &element, elements) {
		result.values << QVariant::fromValue(element.values);
	}

	return result;
}

Rpn::CodeThread CodeGenerator::packVector(const Rpn::Vector &vector)
{
	Rpn::Vector a = vector.values.at(0).value<Rpn::Vector>();
	Rpn::Operand operand(Rpn::OperandVector, QVariant::fromValue(vector));
	Rpn::CodeThread result;
	result << Rpn::Element(Rpn::ElementOperand, QVariant::fromValue(operand));

	return result;
}

// assuming that all arguments are expected to be of the Number type
bool CodeGenerator::isFunctionSignatureSuitable(const QString &functionName, int argumentsCount)
{
	const QList<Rpn::Argument> &functionArguments = functionFormalArguments(functionName);

	if (argumentsCount != Rpn::ArbitraryArgumentsCount) {
		if (functionArguments.size() != argumentsCount) {
			return false;
		}
	}

	for (int i = 0; i < functionArguments.size(); ++i) {
		if (functionArguments.at(i).type != Rpn::OperandNumber) {
			return false;
		}
	}

	return true;
}

void CodeGenerator::checkFormalActualArgumentsCompliance(Rpn::Argument formalArgument,
	const Rpn::CodeThread &actualArgumentCodeThread)
{
	if (formalArgument.type != codeThreadExpressionType(actualArgumentCodeThread)) {
		THROW(EParsing()); // type of the exception is to be more precise
	}

	if (formalArgument.type == Rpn::OperandFunctionName) {

		// make sure that the function passed as an argument has appropriate signature

		// (formalArgument.type == OperandFunctionName) =>
		//	=>	(codeThreadExpressionType(actualArgumentCodeThread) == OperandFunctionName)
		Q_ASSERT(actualArgumentCodeThread.size() == 1);

		Rpn::Element element = actualArgumentCodeThread.last();
		Q_ASSERT(element.type == Rpn::ElementOperand);
		Rpn::Operand operand = element.value.value<Rpn::Operand>();
		QString passedFunctionName = operand.value.value<QString>();
		if (!isFunctionSignatureSuitable(passedFunctionName, formalArgument.info.value<int>())) {			
			THROW(EIncorrectFunctionArgument(passedFunctionName));
		}
	}
}

QList<Rpn::Argument> CodeGenerator::functionFormalArguments(const QString &functionName)
{
	if (m_document->containsFunction(functionName)) {
		return m_document->functionArguments(functionName);
	}
	else if (BuiltIn::Function::functions().contains(functionName)) {
		return BuiltIn::Function::functions().value(functionName)->requiredArguments();
	}
	else {
		THROW(EUndeclaredUsed(functionName, EUndeclaredUsed::Function));
	}
}

Rpn::OperandType CodeGenerator::codeThreadExpressionType(const Rpn::CodeThread &thread)
{
	/*
		The idea is simple.

		If the thread has only one element, than this element
		should be Rpn::ElementOperand or Rpn::ElementConstant and so we have the type.

		If the thread has more than one element, the last element should be Rpn::ElementFunctionCall
		and we have the type again as we know what the function returns.

		In any other case the thread is incorrect.
	*/

	if (thread.size() == 0) {
		THROW(EIncorrectRpnCode());
	}

	// operand, constant or formal argument -- the only element
	else if (thread.size() == 1) {

		Rpn::Element element = thread.last();

		switch (element.type) {

			case Rpn::ElementOperand:
				return element.value.value<Rpn::Operand>().type;

			case Rpn::ElementConstant:
				// constants can currently be only of the Number type
				return Rpn::OperandNumber;

			case Rpn::ElementArgument:
				// user-defined functions currently take only Numbers as arguments,
				return Rpn::OperandNumber;

			default:
				THROW(EIncorrectRpnCode());
		}
	}

	// function call
	else {
		Rpn::Element element = thread.last();
		if (element.type != Rpn::ElementFunctionCall) {
			THROW(EIncorrectRpnCode());
		}

		// define the type of the result of the function call
		QString functionName = element.value.value<QString>();
		return defineFunctionReturnType(functionName);
	}
}

Rpn::OperandType CodeGenerator::defineFunctionReturnType(const QString &functionName)
{
	if (m_document->containsFunction(functionName)) {
		// user-defined functions can currently return only the Number type
		return Rpn::OperandNumber;
	}

	else if (BuiltIn::Function::functions().contains(functionName)) {
		return BuiltIn::Function::functions().value(functionName)->returnValueType();
	}

	else {
		THROW(EIncorrectRpnCode());
	}
}

