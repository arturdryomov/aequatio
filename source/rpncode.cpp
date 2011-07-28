#include "rpncode.h"

RpnOperand::RpnOperand(RpnOperandType type_, const QVariant &value_) :
	type(type_), value(value_)
{
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
