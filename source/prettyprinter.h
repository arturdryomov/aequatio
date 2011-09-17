#ifndef PRETTYPRINTER_H
#define PRETTYPRINTER_H

#include "rpncode.h"

#include <QObject>

class PrettyPrinter : public QObject
{
	Q_OBJECT
public:
	explicit PrettyPrinter(QObject *parent = 0);
	QString process(const RpnCodeThread &codeThread);
	QString process(const RpnFunction &function);
};

#endif // PRETTYPRINTER_H
