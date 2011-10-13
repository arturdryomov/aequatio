#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "builtinfunction.h"

namespace BuiltInFunctions {

// Common class for operations working in algorithms
class MathUtils
{
public:
	static QList<Number> multiplyVectorByNumber(QList<Number> vector, Number number);
	static QList<Number> subtractVectorFromVector(QList<Number> source, QList<Number> subtrahend);
	static QList<Number> addVectorToVector(QList<Number> source, QList<Number> summand);
	static Number multiplyVectorByVectorScalar(QList<Number> source, QList<Number> factor);
	static Number vectorNorm(QList<Number> vector);
	static QList<Number> divideVectorByNumber(QList<Number> vector, Number divisor);

	static Number countDeterminant(QList<QList<Number> > matrix);
	static void ensureSquareMatrix(const QList<QList<Number> > &matrix);
	static void ensureMatrix(const QList<QList<Number> > &matrix);

	static bool isBetween(Number value, Number left, Number right, bool canBeEqual = false);
	static bool equal(Number number1, Number number2);
	static bool isNull(Number number);
	static Number getNaN();
	static bool isNaN(const Number number);

	static QList<Number> generateRandomNumbers(int count, Number lowerLimit, Number higherLimit);
	static Number getRandomNumber(Number higherLimit);

	static Number machineEpsilon();

private:
	static void swapColumns(QList<QList<Number> > &matrix, int index1, int index2);
};

} // namespace

#endif // MATHUTILS_H
