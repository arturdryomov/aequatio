#include "rpncode.h"
#include "calculatingexceptions.h"

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

// Helper methods. Provide to-from QList<Number> conversion for 1-dimensional vectors.
QList<Number> RpnVector::extractSingleVector(RpnVector vector)
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

RpnVector RpnVector::packageSingleVector(QList<Number> list)
{
	RpnVector result;
	foreach (Number element, list) {
		result.values << QVariant::fromValue(element);
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

