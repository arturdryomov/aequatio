#ifndef RPNCODE_H
#define RPNCODE_H

#include "exceptions.h"
#include "number.h"

#include <QVariant>

// RPN stands for ‘Reverse Polish notation’
namespace Rpn {

// basic arithmetic funciton names
const QString FunctionPlus = "@Plus@";
const QString FunctionMinus = "@Minus@";
const QString FunctionMultiply = "@Multiply@";
const QString FunctionDivide = "@Divide@";
const QString FunctionPower = "@Power@";
const QString FunctionUnaryMinus = "@UnaryMinus@";

enum OperandType {
	OperandNumber,
	OperandVector,
	OperandFunctionName
};

struct Vector {
	Vector(int dimensions_ = 1, const QList<QVariant> &values_ = QList<QVariant>());
	int dimensions;
	QList<QVariant> values; // QVariant is a Number or another QList<QVariant>,
	                        // depending on dimensions.
	QString toString();
	static QList<Number> toOneDimensional(Vector vector);
	static Vector fromOneDimensional(QList<Number> list);
	static QList<QList<Number> > toTwoDimensional(Vector vector);
	static Vector fromTwoDimensional(QList<QList<Number> >);
};

struct Operand
{
	Operand(OperandType type_ = Rpn::OperandNumber, const QVariant &value_ = QVariant());
	QString toString();
	bool operator ==(const Rpn::Operand &another);

	OperandType type;
	/* type == Rpn::OperandNumber -- value is a number, type Number,
		OperandVector -- value is Vector,
		OperandFunctionName -- value is a function name (QString) */
	QVariant value;
};

enum ElementType {
	ElementOperand,
	ElementConstant,
	ElementFunctionCall,
	ElementArgument
};

struct Element {
	Element(ElementType type_ = ElementOperand, const QVariant &value_ = QVariant());
	bool operator ==(const Element &another);

	ElementType type;
	/* type == ElementOperand -- value is of type Operand
	  ElementConstant -- value is constant name, QString
	  ElementFunctionCall -- value is function name, QString
	  ElementArgument -- value is argument name, QString
  */
	QVariant value;
};

typedef QList<Element> CodeThread;

// this is not for Rpn::Element with type == Rpn::ElementArgument,
// this is for Rpn::Function
struct Argument {
	Argument(OperandType type_ = Rpn::OperandNumber, const QString &name_ = QString(),
		const QVariant &info_ = QVariant());
	bool operator ==(const Argument &another);

	OperandType type;
	QString name;
	// This is a storage for additional information that depends on type.
	// type == Rpn::OperandNumber -- nothing.
	// OperandVector -- nothing.
	// OperandFunctionName -- number of function arguments, assuming that all this arguments
	//	are of OperandNumber type.
	QVariant info;
};

const int ArbitraryArgumentsCount = -1;

struct Function {
	QList<Rpn::Argument> arguments;
	CodeThread codeThread;
};

} // namespace Rpn

Q_DECLARE_METATYPE(Number)
Q_DECLARE_METATYPE(Rpn::ElementType)
Q_DECLARE_METATYPE(Rpn::OperandType)
Q_DECLARE_METATYPE(Rpn::Vector)
Q_DECLARE_METATYPE(Rpn::Operand)
Q_DECLARE_METATYPE(Rpn::Argument)

#endif // RPNCODE_H
