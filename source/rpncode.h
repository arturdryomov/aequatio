#ifndef RPNCODE_H
#define RPNCODE_H

#include "incorrectinputexceptions.h"
#include "number.h"

#include <QVariant>

// RPN stands for ‘Reverse Polish notation’
namespace Rpn {

// basic arithmetic function names
const QString FunctionPlus = "@Plus@";
const QString FunctionMinus = "@Minus@";
const QString FunctionMultiply = "@Multiply@";
const QString FunctionDivide = "@Divide@";
const QString FunctionPower = "@Power@";
const QString FunctionUnaryMinus = "@UnaryMinus@";

// main function name
const QString FunctionMain = "@Main@";

enum OperandType {
	OperandNumber,
	OperandVector,
	OperandFunctionName,
	OperandIncorrect
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
	Operand(OperandType type_ = OperandNumber, const QVariant &value_ = QVariant());
	QString toString();
	bool operator ==(const Operand &another);

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

// These exceptions can be thrown when working with already built
// RPN code.

class EWrongVectorDimension : public EIncorrectInput
{
public:
	EWrongVectorDimension(int expectedDimension, int actualDimension);
	QString message();
private:
	int m_expectedDimension;
	int m_actualDimension;
};

class EIncorrectRpnCode : public EInternal
{
	/* Reason for all current non-due-to-user-input exceptions in calculator
	is incorrect RPN code. No need to be more concrete at this moment.
	There will be not a problem to derive from EIncorrectRpnCode to provide
	more specific info if necessary. */
};

Q_DECLARE_METATYPE(Number)
Q_DECLARE_METATYPE(Rpn::ElementType)
Q_DECLARE_METATYPE(Rpn::OperandType)
Q_DECLARE_METATYPE(Rpn::Vector)
Q_DECLARE_METATYPE(Rpn::Operand)
Q_DECLARE_METATYPE(Rpn::Argument)

#endif // RPNCODE_H
