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