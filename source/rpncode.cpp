#include "rpncode.h"
#include "builtin/mathutils.h"

namespace Rpn {

Vector::Vector(int dimensions_, const QList<QVariant> &values_) :
	dimensions(dimensions_), values(values_)
{
}

QString Vector::toString()
{
	QString result = "[";

	if (dimensions == 1) {
		QListIterator<QVariant> iterator(values);
		while (iterator.hasNext()) {
			result += numberToString(iterator.next().value<Number>());
			if (iterator.hasNext()) {
				result += ", ";
			}
		}
	}

	else {
		QListIterator<QVariant> iterator(values);
		while (iterator.hasNext()) {
			Vector element(dimensions - 1, iterator.next().value<QList<QVariant> >());
			result += element.toString();
			if (iterator.hasNext()) {
				result += ", ";
			}
		}
	}

	result += "]";

	return result;
}

// Helper methods. Provide to-from QList<Number> conversion for 1- and 2-dimensional vectors.

QList<Number> Vector::toOneDimensional(Vector vector)
{
	if (vector.dimensions != 1) {
		THROW(EWrongVectorDimension(1, vector.dimensions));
	}

	QList<Number> result;
	foreach (QVariant element, vector.values) {
		result << element.value<Number>();
	}

	return result;
}

Vector Vector::fromOneDimensional(QList<Number> list)
{
	Rpn::Vector result;
	foreach (Number element, list) {
		result.values << QVariant::fromValue(element);
	}

	return result;
}

QList<QList<Number> > Vector::toTwoDimensional(Vector vector)
{
	Rpn::Vector a = vector.values.at(0).value<Rpn::Vector>();
	if (vector.dimensions != 2) {
		THROW(EWrongVectorDimension(2, vector.dimensions));
	}

	QList<QList<Number> > result;
	foreach (QVariant elementVector, vector.values) {
		QVariantList variantList = elementVector.value<QVariantList>();
		QList<Number> numberList;
		foreach (QVariant elementNumber, variantList) {
			numberList << elementNumber.value<Number>();
		}
		result << numberList;
	}

	return result;
}

Vector Vector::fromTwoDimensional(QList<QList<Number> > list)
{
	Vector result(2);

	foreach (QList<Number> elementList, list) {
		QVariantList variantList;
		foreach (Number number, elementList) {
			variantList <<QVariant::fromValue(number);
		}
		result.values << QVariant::fromValue(variantList);
	}

	return result;
}

QList<QList<QList<Number> > > Vector::toThreeDimensional(Vector vector)
{
	if (vector.dimensions != 3) {
		THROW(EWrongVectorDimension(3, vector.dimensions));
	}

	QList<QList<QList<Number> > > result;
	foreach (QVariant containterElement, vector.values) {
		QList<QList<Number> > containerList;
		QVariantList containerVariantList = containterElement.value<QVariantList>();
		foreach (QVariant elementVector, containerVariantList) {
			QVariantList variantList = elementVector.value<QVariantList>();
			QList<Number> numberList;
			foreach (QVariant elementNumber, variantList) {
				numberList << elementNumber.value<Number>();
			}
			containerList << numberList;
		}
		result << containerList;
	}

	return result;
}

Vector Vector::fromThreeDimensional(QList<QList<QList<Number> > > list)
{
	Vector result(3);

	foreach (QList<QList<Number> > containerElement, list) {
		QVariantList containerList;
		foreach (QList<Number> elementList, containerElement) {
			QVariantList variantList;
			foreach (Number number, elementList) {
				variantList << QVariant::fromValue(number);
			}
			containerList << QVariant::fromValue(variantList);
		}
		result.values << QVariant::fromValue(containerList);
	}

	return result;
}

Operand::Operand(OperandType type_, const QVariant &value_) :
	type(type_), value(value_)
{
}

QString Operand::toString() const
{
	switch (type) {

		case OperandNumber:
			return numberToString(value.value<Number>());

		case OperandFunctionName:
			return value.value<QString>();

		case OperandVector:
			return value.value<Rpn::Vector>().toString();

		case OperandIncorrect:
			return "Ø";

		default:
			THROW(EIncorrectRpnCode());
	}
}

bool Operand::operator ==(const Rpn::Operand &another)
{
	return (type == another.type)
		&& (value == another.value);
}

Element::Element(ElementType type_, const QVariant &value_) :
	type(type_), value(value_)
{
}

bool Element::operator ==(const Element &another)
{
	return (type == another.type)
		&& (value == another.value);
}

Argument::Argument(OperandType type_, const QString &name_, const QVariant &info_) :
	type(type_), name(name_), info(info_)
{
}

bool Argument::operator ==(const Argument &another)
{
	return (name == another.name)
		&& (type == another.type)
	&& (info == another.info);
}

} // namespace Rpn


// EWrongVectorDimension class methods

EWrongVectorDimension::EWrongVectorDimension(int expectedDimension, int actualDimension) :
	m_expectedDimension(expectedDimension),
	m_actualDimension(actualDimension)
{
}

QString EWrongVectorDimension::message()
{
	return tr("%1-dimensional vector was expected but %2-dimensional was passed.")
		.arg(m_expectedDimension)
		.arg(m_actualDimension);
}

QString ENotSquareMatrix::message()
{
	return tr("Matrix must be square.");
}

QString ENotMatrix::message()
{
	return tr("Matrix expected.");
}

QString ENotSameSizeMatrices::message()
{
	return tr("Matrices should be of the same size.");
}

QString ENotCorrespondingMatricesSizes::message()
{
	return tr("Matrices should have corresponding sizes.");
}
