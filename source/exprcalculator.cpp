#include "exprcalculator.h"
#include "calculatingexceptions.h"

#include <QStack>
#include <QStringList>

#define _USE_MATH_DEFINES
#include <qmath.h>

ExprCalculator::ExprCalculator(QObject *parent) : QObject(parent)
{
	initializeBuiltInFunctions();
	initializeBuiltInConstants();
}

ExpressionDescription ExprCalculator::calculate(const RpnCodeThread &thread)
{
	RpnFunction function = {QList<QString>(), thread};
	m_functions.insert(RpnFunctionMain, function);
	ExpressionDescription description = {rpnCodeThreadToString(thread),
		calculateFunction(RpnFunctionMain, QList<Number>())};

	return description;
}

Number ExprCalculator::calculateFunction(QString functionName, QList<Number> functionArguments)
{
	QStack<Number> calculationStack;

	RpnCodeThread currentThread = m_functions.value(functionName).codeThread;

	// Find and extract

	foreach (RpnElement element, currentThread) {

		switch (element.type) {
			// Just push to numbers stack
			case RpnElementOperand:
				calculationStack.push(element.value.value<Number>());
				break;

			// Get number and push to stack
			case RpnElementArgument: {
				int argumentOrdinalNumber;
				RpnFunction function = m_functions.value(functionName);
				argumentOrdinalNumber = function.arguments.indexOf(element.value.toString());
				calculationStack.push(functionArguments[argumentOrdinalNumber]);
				break;
			}

			// Find constant and push its value
			case RpnElementConstant:
				if (m_builtInConstants.contains(element.value.toString())) {
					calculationStack.push(m_builtInConstants.value(element.value.toString()));
				}
				else if (m_constants.contains(element.value.toString())) {
					calculationStack.push(m_constants.value(element.value.toString()));
				}
				else {
					THROW(EIncorrectRpnCode());
				}
				
				break;

			// Find function and call it
			case RpnElementFunction: {
				QString calledFunctionName = element.value.toString();

				if (m_builtInFunctions.contains(calledFunctionName)) {
					QList<Number> actualArguments;
					for (int i = 0; i < m_builtInFunctions.value(calledFunctionName); i++) {
						actualArguments.prepend(calculationStack.pop());
					}
					Number result = calculateBuiltInFunction(calledFunctionName, actualArguments);
					calculationStack.push(result);
				}

				else if (m_functions.contains(calledFunctionName)) {
					QList<Number> actualArguments;
					for (int i = 0; i < m_functions.value(calledFunctionName).arguments.count(); i++) {
						actualArguments.prepend(calculationStack.pop());
					}
					Number result = calculateFunction(calledFunctionName, actualArguments);
					calculationStack.push(result);
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
	return calculationStack.pop();
}

Number ExprCalculator::calculateBuiltInFunction(QString functionName, QList<Number> functionArguments)
{
	if (functionName == RpnFunctionPlus) {
		return functionArguments[0] + functionArguments[1];
	} 
	else if (functionName == RpnFunctionMinus) {
		return functionArguments[0] - functionArguments[1];
	} 
	else if (functionName == RpnFunctionMultiply) {
		return functionArguments[0] * functionArguments[1];
	} 
	else if (functionName == RpnFunctionDivide) {
		return functionArguments[0] / functionArguments[1];
	} 
	else if (functionName == RpnFunctionPower) {
		return qPow(functionArguments[0],  functionArguments[1]);
	} 
	else if (functionName == RpnFunctionUnaryMinus) {
		return -functionArguments[0];
	}
	else if (functionName == Sine) {
		return qSin(functionArguments[0]);
	}
	else if (functionName == Cosine) {
		return qCos(functionArguments[0]);
	}
	else if (functionName == Tangent) {
		return qTan(functionArguments[0]);
	} else {
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
	description.arguments = functionCode.arguments;
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
				part.text = numberToString(element.value.value<Number>());
				part.priority = PriorityHighest;
				break;

			case RpnElementConstant:
				part.text = element.value.toString();
				part.priority = PriorityHighest;
				break;

			case RpnElementArgument: {
				part.text = element.value.toString();
				part.priority = PriorityHighest;
				break;
			}

			case RpnElementFunction: {
				QString functionName = element.value.toString();

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
						argumentsCount = m_builtInFunctions.value(functionName);
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
	m_builtInFunctions.insert(RpnFunctionMain, 0);
	m_builtInFunctions.insert(RpnFunctionPlus, 2);
	m_builtInFunctions.insert(RpnFunctionMinus, 2);
	m_builtInFunctions.insert(RpnFunctionMultiply, 2);
	m_builtInFunctions.insert(RpnFunctionDivide, 2);
	m_builtInFunctions.insert(RpnFunctionPower, 2);
	m_builtInFunctions.insert(RpnFunctionUnaryMinus, 1);
	m_builtInFunctions.insert(Sine, 1);
	m_builtInFunctions.insert(Cosine, 1);
	m_builtInFunctions.insert(Tangent, 1);
}

void ExprCalculator::initializeBuiltInConstants()
{
	m_builtInConstants.insert(Pi, M_PI);
	m_builtInConstants.insert(E, M_E);
}
