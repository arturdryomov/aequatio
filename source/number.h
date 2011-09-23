#ifndef NUMBER_H
#define NUMBER_H

#include "exceptions.h"

#include <QtGlobal>

typedef qreal Number;

class EConversionToNumber : public EInternal
{
public:
	EConversionToNumber(const QString &numberRepresentation);
	QString message();
	QString m_numberRepresentation;
};

// This is to unify Number to string and string to Number converting.
QString numberToString(const Number number);
Number stringToNumber(const QString &str);

#endif // NUMBER_H
