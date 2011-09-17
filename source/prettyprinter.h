#ifndef PRETTYPRINTER_H
#define PRETTYPRINTER_H

#include "rpncode.h"

#include <QObject>

class PrettyPrinter
{
public:
	PrettyPrinter();
	QString process(const RpnCodeThread &codeThread);
	QString process(const RpnFunction &function);
};

#endif // PRETTYPRINTER_H
