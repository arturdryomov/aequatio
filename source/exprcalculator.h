#ifndef EXPRCALCULATOR_H
#define EXPRCALCULATOR_H

#include <QObject>
#include <QVariant>

typedef double Number;
// Q_DECLARE_METATYPE(Number);

enum RpnElementType { RpnOperation, RpnOperand };
enum OperationType { OperationPlus, OperationMinus, OperationMultiply,
	OperationDivide };

struct RpnElement // RPN stands for ‘reverse Polish notation’
{
	RpnElementType type;
	QVariant value; // this will hold a Number or a OperationType value
};

struct RpnCode
{
	QList<RpnElement> elements; // there will be a number of lists here: main thread, functions…
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

#endif // EXPRCALCULATOR_H
