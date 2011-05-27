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
	RpnElementFunction // function call, value is its name (QString)
};

struct RpnElement
{
	RpnElementType type;
	QVariant value; // this will hold a Number, parameter’s ordinal number or function name
};

typedef QList<RpnElement> RpnCodeThread; // contains linear RPN code to calculate

struct RpnFunction {
	int parametersNumber;
	RpnCodeThread codeThread;
};

typedef QHash<QString, RpnFunction> RpnCode;

const QString RpnFunctionMain = "@Main@";
const QString RpnFunctionPlus = "@Plus@";
const QString RpnFunctionMinus = "@Minus@";
const QString RpnFunctionMultiply = "@Multiply@";
const QString RpnFunctionDivide = "@Divide@";
const QString RpnFunctionPower = "@Power@";

class ExprCalculator : public QObject
{
	Q_OBJECT	
public:
	explicit ExprCalculator(QObject *parent = 0);
	Number calculate(const RpnCode &code);

	bool isBuiltInFunction(const QString &functionName);
	int builtInFunctionArgumentsNumber(const QString &functionName);
	Number calculateFunction(QString functionName, QVector<Number> functionArguments);
	Number calculateBuiltInFunction(QString functionName, QVector<Number> functionArguments);
	
	class Exception
	{
	public:
		explicit Exception(const QString &message) : m_message(message) {}
		QString message() { return m_message; }
	private:		
		QString m_message;
	};
private:
	QHash<QString, int> m_standardFunctions;
	RpnCode m_rpnCode;

	void initializeStandardFunctions();
};

Q_DECLARE_METATYPE(Number)
Q_DECLARE_METATYPE(RpnElementType)

#endif // EXPRCALCULATOR_H
