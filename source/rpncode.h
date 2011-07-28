#ifndef RPNCODE_H
#define RPNCODE_H

#include <QVariant>

typedef qreal Number;

// This is to unify Number to string converting.
inline QString numberToString(const Number number)
{
	return QString::number(number).replace("-", "−");
}

// RPN stands for ‘Reverse Polish notation’

enum RpnOperandType {
	RpnOperandNumber,
	RpnOperandFunctionName
};

struct RpnOperand
{
	RpnOperand(RpnOperandType type_ = RpnOperandNumber, const QVariant &value_ = QVariant());
	bool operator ==(const RpnOperand &another);

	RpnOperandType type;
	/* type == RpnOperandNumber -- value is a number, type Number,
	  RpnOperandFunctionName -- value is a function name (QString) */
	QVariant value;
};

enum RpnElementType {
	RpnElementOperand,
	RpnElementConstant,
	RpnElementFunctionCall,
	RpnElementArgument
};

struct RpnElement {
	RpnElement(RpnElementType type_ = RpnElementOperand, const QVariant &value_ = QVariant::fromValue(RpnOperand()));
	bool operator ==(const RpnElement &another);

	RpnElementType type;
	/* type == RpnElementOperand -- value is of type RpnOperand
	  RpnElementConstant -- value is constant name, QString
	  RpnElementFunctionCall -- value is function name, QString
	  RpnElementArgument -- value is argument name, QString
  */
	QVariant value;
};

// без изменений
typedef QList<RpnElement> RpnCodeThread;

// this is not for RpnElement with type == RpnElementArgument,
// this is for RpnFunction
struct RpnArgument {
	RpnArgument(RpnOperandType type_ = RpnOperandNumber, const QString &name_ = QString(),
		const QVariant &info_ = QVariant());
	bool operator ==(const RpnArgument &another);

	RpnOperandType type;
	QString name;
	// This is a storage for additional information that depends on type.
	// type == RpnOperandNumber -- nothing.
	// RpnOperandFunctionName -- number of function arguments, assuming that all this arguments
	//	are of RpnOperandNumber type.
	QVariant info;
};

struct RpnFunction {
	QList<RpnArgument> arguments;
	RpnCodeThread codeThread;
};

Q_DECLARE_METATYPE(Number)
Q_DECLARE_METATYPE(RpnElementType)
Q_DECLARE_METATYPE(RpnOperandType)
Q_DECLARE_METATYPE(RpnOperand)
Q_DECLARE_METATYPE(RpnArgument)

#endif // RPNCODE_H
