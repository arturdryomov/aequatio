#include "exprcalculator.h"
#include "calculatingexceptions.h"

#include <QStack>
#include <QStringList>

#define _USE_MATH_DEFINES
#include <qmath.h>

bool operator ==(const RpnArgument &a1, const RpnArgument &a2)
{
	return (a1.name == a2.name)
		&& (a1.type == a2.type)
		&& (a1.info == a2.info);
}

ExprCalculator::ExprCalculator(QObject *parent) : QObject(parent)
{
	initializeBuiltInFunctions();
	initializeBuiltInConstants();
}

ExpressionDescription ExprCalculator::calculate(const RpnCodeThread &thread)
{
	RpnFunction function = {QList<RpnArgument>(), thread};
	m_functions.insert(RpnFunctionMain, function);
	ExpressionDescription description = {rpnCodeThreadToString(thread),
		calculateFunction(RpnFunctionMain, QList<RpnOperand>())};

	return description;
}

Number ExprCalculator::calculateFunction(QString functionName, QList<RpnOperand> functionArguments)
{
	QStack<RpnOperand> calculationStack;

	RpnCodeThread currentThread = m_functions.value(functionName).codeThread;

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
				RpnFunction function = m_functions.value(functionName);

				for (int i = 0; i < function.arguments.count(); ++i) {
					// find ordinal number of the argument with name argumentName
					// and substitute it with actual value from funtionArguments
					if (argumentName == function.arguments.at(i).name) {
						calculationStack.push(functionArguments[i]);
						break;
					}
				}
				break;
			}

			// Find constant and push its value
			case RpnElementConstant: {
				RpnOperand operand = {RpnOperandNumber, QVariant()};
				if (m_builtInConstants.contains(element.value.toString())) {
					operand.value = m_builtInConstants.value(element.value.toString());
				}
				else if (m_constants.contains(element.value.toString())) {
					operand.value = m_constants.value(element.value.toString());
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

				if (m_builtInFunctions.contains(callingFunctionName)) {
					QList<RpnOperand> operands;
					for (int i = 0; i < m_builtInFunctions.value(callingFunctionName).count(); i++) {
						operands.prepend(calculationStack.pop());
					}
					Number result = calculateBuiltInFunction(callingFunctionName, operands);
					RpnOperand operand = {RpnOperandNumber, result};
					calculationStack.push(operand);
				}

				else if (m_functions.contains(callingFunctionName)) {
					QList<RpnOperand> RpnOperands;
					for (int i = 0; i < m_functions.value(callingFunctionName).arguments.count(); i++) {
						RpnOperands.prepend(calculationStack.pop());
					}
					Number result = calculateFunction(callingFunctionName, RpnOperands);
					RpnOperand operand = {RpnOperandNumber, result};
					calculationStack.push(operand);
				}

				else {
					THROW(EIncorrectRpnCode());
				}
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
	if (result.type != RpnOperandNumber) {
		THROW(EIncorrectRpnCode());
	}

	return result.value.value<Number>();
}

Number ExprCalculator::calculateBuiltInFunction(QString functionName, QList<RpnOperand> functionArguments)
{
	// We will not check for argument types here. This code will soon be moved from ExprCalculator
	// and respective checks will be made at that time.

	if (functionName == RpnFunctionPlus) {
		return functionArguments[0].value.value<Number>() + functionArguments[1].value.value<Number>();
	} 
	else if (functionName == RpnFunctionMinus) {
		return functionArguments[0].value.value<Number>() - functionArguments[1].value.value<Number>();
	} 
	else if (functionName == RpnFunctionMultiply) {
		return functionArguments[0].value.value<Number>() * functionArguments[1].value.value<Number>();
	} 
	else if (functionName == RpnFunctionDivide) {
		return functionArguments[0].value.value<Number>() / functionArguments[1].value.value<Number>();
	} 
	else if (functionName == RpnFunctionPower) {
		return qPow(functionArguments[0].value.value<Number>(),  functionArguments[1].value.value<Number>());
	} 
	else if (functionName == RpnFunctionUnaryMinus) {
		return -functionArguments[0].value.value<Number>();
	}
	else if (functionName == Sine) {
		return qSin(functionArguments[0].value.value<Number>());
	}
	else if (functionName == Cosine) {
		return qCos(functionArguments[0].value.value<Number>());
	}
	else if (functionName == Tangent) {
		return qTan(functionArguments[0].value.value<Number>());
	}
	else if (functionName == "test_new_function") {
		QString functionToCall = functionArguments[0].value.value<QString>();
		QList<RpnOperand> calledFunctionArguments;
		calledFunctionArguments << functionArguments[1];

		if (m_functions.contains(functionToCall)) {
			return calculateFunction(functionToCall, calledFunctionArguments);
		}
		else if (m_builtInFunctions.contains(functionToCall)) {
			return calculateBuiltInFunction(functionToCall, calledFunctionArguments);
		}
		else {
			THROW(EInternal());
		}
	}
	else {
		THROW(EIncorrectRpnCode());
	}
	
	// Compiler needs this useless thing
	return 0;
}

FunctionDescription ExprCalculator::functionDescription(const QString &functionName)
{
	RpnFunction functionCode = m_functions.value(functionName);
	FunctionDescription description;
	description.name = functionName;
	foreach (RpnArgument argument, functionCode.arguments) {
		description.arguments << argument.name;
	}
	description.body = rpnCodeThreadToString(functionCode.codeThread);
	return description;
}

QString ExprCalculator::operandToText(const RpnOperand &operand)
{
	switch (operand.type) {
		case RpnOperandNumber:
			return numberToString(operand.value.value<Number>());
		case RpnOperandFunctionName:
			return operand.value.value<QString>();
		default:
			THROW(EIncorrectRpnCode());
	}
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
				part.text = operandToText(element.value.value<RpnOperand>());
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
					if (m_builtInFunctions.contains(functionName)) {
						argumentsCount = m_builtInFunctions.value(functionName).count();
					}
					else if (m_functions.contains(functionName)) {
						argumentsCount = m_functions.value(functionName).arguments.count();
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
	if (m_builtInConstants.contains(name)) {
		THROW(EBuiltInRedifinition(name, EBuiltInRedifinition::Constant));
	}

	m_constants.insert(name, value);
	emit constantsListChanged();

	ConstantDescription constantDescription = {name, value};
	return constantDescription;
}

FunctionDescription ExprCalculator::addFunction(const QString &name, const RpnFunction &function)
{
	if (m_builtInFunctions.contains(name)) {
		THROW(EBuiltInRedifinition(name, EBuiltInRedifinition::Function));
	}

	if (m_functionNames.contains(name)) {
		m_functionNames.removeOne(name);
	}
	m_functionNames.append(name);
	m_functions.insert(name, function);
	emit functionsListChanged();

	return functionDescription(name);
}

bool ExprCalculator::isFunction(const QString &name)
{
	return (m_functions.contains(name) || m_builtInFunctions.contains(name));
}

bool ExprCalculator::isConstant(const QString &name)
{
	return (m_constants.contains(name) || m_builtInConstants.contains(name));
}

int ExprCalculator::functionArgumentsCount(const QString &name)
{
	if (!isFunction(name)) {
		THROW(EIncorrectRpnCode());
	}

	if (m_functions.contains(name)) {
		return m_functions.value(name).arguments.count();
	}
	else {
		return m_builtInFunctions.value(name).count();
	}
}

QList<RpnArgument> ExprCalculator::functionArguments(const QString &name)
{
	if (!isFunction(name)) {
		THROW(EIncorrectRpnCode());
	}

	if (m_functions.contains(name)) {
		return m_functions.value(name).arguments;
	}
	else {
		return m_builtInFunctions.value(name);
	}
}

QList<ConstantDescription> ExprCalculator::constantsList()
{
	QList<ConstantDescription> constantsList;

	QHashIterator<QString, Number> i(m_constants);
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

	foreach (QString functionName, m_functionNames) {
		if (functionName != RpnFunctionMain) {
			FunctionDescription function = functionDescription(functionName);
			functionsList << function;
		}
	}

	return functionsList;
}

void ExprCalculator::initializeBuiltInFunctions()
{
	QList<RpnArgument> arguments;

	// no arguments expected
	m_builtInFunctions.insert(RpnFunctionMain, arguments);

	// one Number argument expected
	RpnArgument argumentNumber = {RpnOperandNumber, QString(), QVariant()};
	arguments << argumentNumber;
	m_builtInFunctions.insert(RpnFunctionUnaryMinus, arguments);
	m_builtInFunctions.insert(Sine, arguments);
	m_builtInFunctions.insert(Cosine, arguments);
	m_builtInFunctions.insert(Tangent, arguments);

	// two Number argument expected
	arguments << argumentNumber;
	m_builtInFunctions.insert(RpnFunctionPlus, arguments);
	m_builtInFunctions.insert(RpnFunctionMinus, arguments);
	m_builtInFunctions.insert(RpnFunctionMultiply, arguments);
	m_builtInFunctions.insert(RpnFunctionDivide, arguments);
	m_builtInFunctions.insert(RpnFunctionPower, arguments);

	arguments.clear();
	// 1 is argument count in function that is passed as and argument to "test_new_function"
	RpnArgument argumentFunction = {RpnOperandFunctionName, QString(), QVariant::fromValue(1)};
	arguments << argumentFunction << argumentNumber;
	m_builtInFunctions.insert("test_new_function", arguments);
}

void ExprCalculator::initializeBuiltInConstants()
{
	m_builtInConstants.insert(Pi, M_PI);
	m_builtInConstants.insert(E, M_E);
}
