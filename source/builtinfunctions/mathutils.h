#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "builtinfunction.h"

class MathUtils
{
public:
	static QList<Number> multiplyVectorByNumber(QList<Number> vector, Number number);
	static QList<Number> subtractVectorFromVector(QList<Number> source, QList<Number> subtrahend);
	static QList<Number> addVectorToVector(QList<Number> source, QList<Number> summand);
	static Number multiplyVectorByVectorScalar(QList<Number> source, QList<Number> factor);
	static Number vectorNorm(QList<Number> vector);
	static QList<Number> divideVectorByNumber(QList<Number> vector, Number divisor);

	static Number countDeterminant(QVector<QVector<Number> > matrix);
};

#endif // MATHUTILS_H
