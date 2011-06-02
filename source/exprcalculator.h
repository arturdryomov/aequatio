#ifndef EXPRCALCULATOR_H
#define EXPRCALCULATOR_H

#include <QHash>
#include <QObject>
#include <QVariant>
#include <QMetaType>
#include <QList>

typedef qreal Number;

// RPN stands for ‘Reverse Polish notation’
enum RpnElementType {
	RpnElementOperand, // operand, value is Number
	RpnElementParam, // parameter, value is ordinal number of the parameter (int)
	RpnElementFunction, // function call, value is its name (QString)
	RpnElementConstant // constant, value is its name (QString)
};

struct RpnElement
{
	RpnElementType type;
	QVariant value; // this will hold a Number, parameter’s ordinal number or function name
};

typedef QList<RpnElement> RpnCodeThread; // contains linear RPN code to calculate

struct RpnFunction {
	int argumentsCount;
	RpnCodeThread codeThread;
};

typedef QHash<QString, RpnFunction> RpnCode;

// main and built-in functions names
const QString RpnFunctionMain = "@Main@";

const QString RpnFunctionPlus = "@Plus@";
const QString RpnFunctionMinus = "@Minus@";
const QString RpnFunctionMultiply = "@Multiply@";
const QString RpnFunctionDivide = "@Divide@";
const QString RpnFunctionPower = "@Power@";

// these are real built in functions and in fact it would be better to hide 
// these definitions from client code
const QString Sine = "sin";
const QString Cosine = "cos";
const QString Tangent = "tan";

// built in constants
const QString Pi = "pi";
const QString E = "e";

class ExprCalculator : public QObject
{
	Q_OBJECT
public:
	explicit ExprCalculator(QObject *parent = nullptr);

	Number calculate(const RpnCodeThread &thread);
	void addConstant(const QString &name, const Number &value);
	void addFunction(const QString &name, const RpnFunction &function);

	bool isFunction(const QString &name);
	bool isConstant(const QString &name);
	int functionArgumentsCount(const QString &name);

	class Exception
	{
	public:
		explicit Exception(const QString &message) : m_message(message) {}
		QString message() { return m_message; }
	private:
		QString m_message;
	};
private:
	QHash<QString, RpnFunction> m_functions;
	QHash<QString, int> m_builtInFunctions;
	QHash<QString, Number> m_constants;
	QHash<QString, Number> m_builtInConstants;
	RpnCodeThread m_rpnCodeThread;

	void initializeBuiltInFunctions();
	void initializeBuiltInConstants();
	Number calculateFunction(QString functionName, QList<Number> functionArguments);
	Number calculateBuiltInFunction(QString functionName, QList<Number> functionArguments);
};

Q_DECLARE_METATYPE(Number)
Q_DECLARE_METATYPE(RpnElementType)

#endif // EXPRCALCULATOR_H
