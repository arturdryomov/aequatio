#include "document.h"
#include "prettyprinter.h"
#include "builtin/function.h"
#include "builtin/constant.h"
#include "rpncode.h"

Document::Document(QObject *parent) : QObject(parent)
{
}

bool Document::containsFunction(const QString &name) const
{
	return m_functions.contains(name);
}

bool Document::containsConstant(const QString &name) const
{
	return m_constants.contains(name);
}

QList<Rpn::Argument> Document::functionArguments(const QString &name) const
{
	if (!m_functions.contains(name)) {
		THROW(EIncorrectRpnCode());
	}

	return m_functions.value(name).arguments;
}

Rpn::Function Document::function(const QString &name) const
{
	if (!m_functions.contains(name)) {
		THROW(EIncorrectRpnCode());
	}

	return m_functions.value(name);
}

Number Document::constant(const QString &name) const
{
	if (!m_constants.contains(name)) {
		THROW(EIncorrectRpnCode());
	}

	return m_constants.value(name);
}

QString Document::prettyPrintedFunction(const QString &name) const
{
	if (!m_functions.contains(name)) {
		THROW(EIncorrectRpnCode());
	}

	PrettyPrinter prettyPrinter(this);
	return prettyPrinter.process(m_functions.value(name), name);
}

QString Document::prettyPrintedConstant(const QString &name) const
{
	if (!m_constants.contains(name)) {
		THROW(EIncorrectRpnCode());
	}

	return QString("%1 = %2")
		.arg(name)
		.arg(numberToString(m_constants.value(name)));
}

QStringList Document::prettyPrintedFunctions() const
{
	QStringList functions;

	QMapIterator<QString, Rpn::Function> i(m_functions);
	while (i.hasNext()) {
		i.next();
			if (i.key() != Rpn::FunctionMain) {
				functions <<  prettyPrintedFunction(i.key());
			}
	}

	return functions;
}

QStringList Document::prettyPrintedConstants() const
{
	QStringList constants;

	QMapIterator<QString, Number> i(m_constants);
	while (i.hasNext()) {
		i.next();
		constants << prettyPrintedConstant(i.key());
	}

	return constants;
}

void Document::addConstant(const QString &name, const Number &value)
{
	if (BuiltIn::Constant::constants().contains(name)) {
		THROW(EBuiltInRedifinition(name, EBuiltInRedifinition::Constant));
	}

	m_constants.insert(name, value);
	emit constantsChanged();
}

void Document::addFunction(const QString &name, const Rpn::Function &function)
{
	if (BuiltIn::Function::functions().contains(name)) {
		THROW(EBuiltInRedifinition(name, EBuiltInRedifinition::Function));
	}

	if (isFunctionUsed(name, function.codeThread)) {
		THROW(ERecursiveFunction(name));
	}

	m_functions.insert(name, function);
	emit functionsChanged();
}

bool Document::isFunctionUsed(const QString &name, const Rpn::CodeThread &codeThread) const
{
	foreach (Rpn::Element element, codeThread) {
		if (element.type != Rpn::ElementFunctionCall) continue;

		QString calledFunctionName = element.value.value<QString>();
		if (!m_functions.contains(calledFunctionName)) continue;

		// Is this our function call
		if (calledFunctionName == name) {
			return true;
		}

		// Check recursively
		Rpn::CodeThread calledFunctionCode = m_functions.value(calledFunctionName).codeThread;
		if (isFunctionUsed(name, calledFunctionCode)) {
			return true;
		}
	}

	return false;
}


// EBuiltInRedifinition class methods

EBuiltInRedifinition::EBuiltInRedifinition(const QString &elementName, EBuiltInRedifinition::ElementType elementType) :
	m_elementName(elementName),
	m_elementType(elementType)
{
}

QString EBuiltInRedifinition::message()
{
	if (m_elementType == Constant) {
		return tr("You cannot declare constant “%1” because there is already "
			"built-in constant with this name.").arg(m_elementName);
	}
	else {
		return tr("You cannot declare function “%1()” because there is already "
			"built-in function with this name.").arg(m_elementName);
	}
}


// ERecursiveFunction class methods

ERecursiveFunction::ERecursiveFunction(const QString &functionName) : m_functionName(functionName)
{
}

QString ERecursiveFunction::message()
{
	return tr("You cannot declare function “%1” this way because its calculation then will lead "
		"to infinite recursive calls.", "ERecursiveFunction").arg(m_functionName);
}
