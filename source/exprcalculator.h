#ifndef EXPRCALCULATOR_H
#define EXPRCALCULATOR_H

#include <QObject>
#include <QVariant>
#include <QMetaType>

typedef double Number;

// RPN stands for ‘Reverse Polish notation’
enum RpnElementType { RpnOperation, RpnOperand };

enum OperationType { OperationPlus, OperationMinus, OperationMultiply, OperationDivide };

struct RpnElement
{
	RpnElementType type;
	QVariant value; // this will hold a Number or an OperationType value
};

typedef QList<RpnElement> RpnCodeThread; // contains linear RPN code to calculate

struct RpnCode
{
	// there will be a number of lists here: main thread, functions. Now its only main thread.
	RpnCodeThread elements; 
};

class ExprCalculator : public QObject
{
	Q_OBJECT	
public:
	explicit ExprCalculator(QObject *parent = 0);
	Number calculate(RpnCode *code);
	class Exception
	{
	public:
		explicit Exception(const QString &message) : m_message(message) {}
		QString message() { return m_message; }
	private:
		QString m_message;
	};
};

Q_DECLARE_METATYPE(Number)
Q_DECLARE_METATYPE(OperationType)

#endif // EXPRCALCULATOR_H
