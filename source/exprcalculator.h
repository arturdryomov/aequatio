#ifndef EXPRCALCULATOR_H
#define EXPRCALCULATOR_H

#include <QHash>
#include <QObject>
#include <QVariant>
#include <QMetaType>
#include <QList>

typedef qreal Number;

// This is to unify Number to string converting.
inline QString numberToString(const Number number)
{
	return QString::number(number).replace("-", "−");
}

// RPN stands for ‘Reverse Polish notation’

enum RpnArgumentType {ArgumentTypeNumber, ArgumentTypeFunction};

struct RpnArgument {
	QString name;
	RpnArgumentType type;
};
bool operator ==(const RpnArgument &a1, const RpnArgument &a2);

enum RpnElementType {
	RpnElementOperand, // operand, value is Number
	RpnElementArgument,// argument, value is of type RpnArgument
	RpnElementFunction,// function call, value is its name (QString)
	RpnElementConstant // constant, value is its name (QString)
};

struct RpnElement
{
	RpnElementType type;
	QVariant value; // this will hold a Number, parameter name, constant name or function name
};

typedef QList<RpnElement> RpnCodeThread; // contains linear RPN code to calculate

struct RpnFunction {
	QList<RpnArgument> arguments;
	RpnCodeThread codeThread;
};

typedef QHash<QString, RpnFunction> RpnCode;


struct ConstantDescription {
	QString name;
	Number value;
};

struct FunctionDescription {
	QString name;
	QList<QString> arguments;
	QString body;
};

struct ExpressionDescription {
	QString expression;
	Number result;
};

// main and built-in functions names
const QString RpnFunctionMain = "@Main@";

const QString RpnFunctionPlus = "@Plus@";
const QString RpnFunctionMinus = "@Minus@";
const QString RpnFunctionMultiply = "@Multiply@";
const QString RpnFunctionDivide = "@Divide@";
const QString RpnFunctionPower = "@Power@";
const QString RpnFunctionUnaryMinus = "@UnaryMinus@";

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
signals:
	void constantsListChanged();
	void functionsListChanged();
public:
	explicit ExprCalculator(QObject *parent = 0);

	ExpressionDescription calculate(const RpnCodeThread &thread);
	ConstantDescription addConstant(const QString &name, const Number &value);
	FunctionDescription addFunction(const QString &name, const RpnFunction &function);

	bool isFunction(const QString &name);
	bool isConstant(const QString &name);
	int functionArgumentsCount(const QString &name);

	QList<ConstantDescription> constantsList();
	QList<FunctionDescription> functionsList();
private:
	QList<QString> m_functionNames; // stores function in order of their declaration
	QHash<QString, RpnFunction> m_functions;
	QHash<QString, int> m_builtInFunctions;
	QHash<QString, Number> m_constants;
	QHash<QString, Number> m_builtInConstants;
	RpnCodeThread m_rpnCodeThread;

	// PartPriority and PartInfo are used in rpnCodeThreadToString() only.
	enum PartPriority {PriorityPlusMinus, PriorityMultiplyDivide, PriorityPower,
		PriorityHighest, PriorityFunction = PriorityHighest, PriorityNumber = PriorityHighest};
	struct PartInfo {
		QString text;
		PartPriority priority;
		void bracesIfGreater(PartPriority externalPriority) {
			if (externalPriority > priority) {
				text = QString("(%1)").arg(text);
			}
		}
		void bracesIfGreaterOrEqual(PartPriority externalPriority) {
			if (externalPriority >= priority) {
				text = QString("(%1)").arg(text);
			}
		}
	};
	QString rpnCodeThreadToString(const RpnCodeThread &codeThread);
	void initializeBuiltInFunctions();
	void initializeBuiltInConstants();

	struct ActualArgument {
		RpnArgumentType type;
		QVariant value; // Number for ArgumentTypeNumber and QString for ArgumentTypeFunction
	};

	Number calculateFunction(QString functionName, QList<ActualArgument> functionArguments);
	Number calculateBuiltInFunction(QString functionName, QList<ActualArgument> functionArguments);
	FunctionDescription functionDescription(const QString &functionName);
};

Q_DECLARE_METATYPE(Number)
Q_DECLARE_METATYPE(RpnElementType)
Q_DECLARE_METATYPE(RpnArgument)
Q_DECLARE_METATYPE(ConstantDescription)
Q_DECLARE_METATYPE(FunctionDescription)
Q_DECLARE_METATYPE(ExpressionDescription)

#endif // EXPRCALCULATOR_H
