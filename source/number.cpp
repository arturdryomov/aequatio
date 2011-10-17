#include "number.h"
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
