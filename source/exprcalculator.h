#ifndef EXPRCALCULATOR_H
#define EXPRCALCULATOR_H

#include <QHash>
#include <QObject>
#include <QVariant>
#include <QMetaType>
#include <QList>

typedef qreal Number;

// RPN stands for ‘Reverse Polish notation’

typedef QList<RpnElement> RpnCodeThread; // contains linear RPN code to calculate
typedef QHash<QString, RpnFunction> RpnCode;

const QString RpnFunctionMain = "@Main@";
const QString RpnFunctionPlus = "@Plus@";
const QString RpnFunctionMinus = "@Minus@";
const QString RpnFunctionMultiply = "@Multiply@";
const QString RpnFunctionDivide = "@Divide@";
const QString RpnFunctionPower = "@Power@";

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

struct RpnFunction {
	int parametersNumber;
	RpnCodeThread codeThread;
};

class ExprCalculator : public QObject
{
	Q_OBJECT	
public:
	explicit ExprCalculator(QObject *parent = 0);
	Number calculate(RpnCode *code);

	bool isBuiltInFunction(const QString &name);
	int builtInFunctionArgumentsNumber(const QString &functionName);
	Number calculateFunction(QString functionName, QVector functionParameters);
	Number calculateBuiltInFunction(QString functionName, QVector functionParameters);
	
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
};

Q_DECLARE_METATYPE(Number)
Q_DECLARE_METATYPE(OperationType)

#endif // EXPRCALCULATOR_H
