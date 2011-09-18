#include "rpncode.h"
#include "calculatingexceptions.h"
#include "builtin/mathutils.h"

QString numberToString(const Number number)
{
	// MathUtils should possibly be move to root from builtin
	if (BuiltIn::MathUtils::isNaN(number)) {
		return ("Ø"); // empty set
	}

	return QString::number(number).replace("-", "−");
}

Number stringToNumber(const QString &str)
{
	bool ok = false;
	QString stringToConvert = str; // the compiler won't allow str.replace() as str is constant reference
	Number result = stringToConvert.replace("−", "-").toDouble(&ok);
	if (!ok) {
		THROW(EConversionToNumber(stringToConvert));
	}
	return result;
}

EConversionToNumber::EConversionToNumber(const QString &numberRepresentation) :
	m_numberRepresentation(numberRepresentation)
{
}

QString EConversionToNumber::message()
{
	return tr("Error occurred while trying to convert “%1” into a number format. Let the developers "
		"know about this, please.").arg(m_numberRepresentation);
}

RpnVector::RpnVector(int dimensions_, const QList<QVariant> &values_) :
	dimensions(dimensions_), values(values_)
{
}

QString RpnVector::toString()
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
			RpnVector element(dimensions - 1, iterator.next().value<QList<QVariant> >());
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

QList<Number> RpnVector::toOneDimensional(RpnVector vector)
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

RpnVector RpnVector::fromOneDimensional(QList<Number> list)
{
	RpnVector result;
	foreach (Number element, list) {
		result.values << QVariant::fromValue(element);
	}

	return result;
}

QList<QList<Number> > RpnVector::toTwoDimensional(RpnVector vector)
{
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

RpnVector RpnVector::fromTwoDimensional(QList<QList<Number> > list)
{
	RpnVector result(2);

	foreach (QList<Number> elementList, list) {
		QVariantList variantList;
		foreach (Number number, elementList) {
			variantList <<QVariant::fromValue(number);
		}
		result.values << QVariant::fromValue(variantList);
	}

	return result;
}

RpnOperand::RpnOperand(RpnOperandType type_, const QVariant &value_) :
	type(type_), value(value_)
{
}

QString RpnOperand::toString()
{
	switch (type) {

		case RpnOperandNumber:
			return numberToString(value.value<Number>());

		case RpnOperandFunctionName:
			return value.value<QString>();

		case RpnOperandVector:
			return value.value<RpnVector>().toString();

		default:
			THROW(EIncorrectRpnCode());
	}
}

bool RpnOperand::operator ==(const RpnOperand &another)
{
	return (type == another.type)
		&& (value == another.value);
}

RpnElement::RpnElement(RpnElementType type_, const QVariant &value_) :
	type(type_), value(value_)
{
}

bool RpnElement::operator ==(const RpnElement &another)
{
	return (type == another.type)
		&& (value == another.value);
}

RpnArgument::RpnArgument(RpnOperandType type_, const QString &name_, const QVariant &info_) :
	type(type_), name(name_), info(info_)
{
}

bool RpnArgument::operator ==(const RpnArgument &another)
{
	return (name == another.name)
		&& (type == another.type)
	&& (info == another.info);
}

