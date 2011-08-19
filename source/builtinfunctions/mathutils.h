#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "builtinfunction.h"

class MathUtils
{
public:
	static QList<Number> productListNumber(QList<Number> list, Number number);
	static QList<Number> diffListList(QList<Number> source, QList<Number> subtractin);
	static QList<Number> sumListList(QList<Number> source, QList<Number> item);
	static Number productListList(QList<Number> source, QList<Number> item);
	static Number modulusList(QList<Number> list);
	static QList<Number> quotientListNumber(QList<Number> source, Number divisor);

	static Number countDeterminant(QVector<QVector<Number> > matrix);
};

#endif // MATHUTILS_H
