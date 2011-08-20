#include "mathutils.h"

QList<Number> MathUtils::productListNumber(QList<Number> list, Number number)
{
	QList<Number> result;

	foreach (Number element, list) {
		result << element * number;
	}

	return result;
}

QList<Number> MathUtils::diffListList(QList<Number> source, QList<Number> subtractin)
{
	QList<Number> result;

	for (int i = 0; i < source.size(); i++) {
		result << source[i] - subtractin[i];
	}

	return result;
}

QList<Number> MathUtils::sumListList(QList<Number> source, QList<Number> item)
{
	QList<Number> result;

	for (int i = 0; i < source.size(); i++) {
		result << source[i] + item[i];
	}

	return result;
}

Number MathUtils::productListList(QList<Number> source, QList<Number> item)
{
	Number result = 0;

	for (int i = 0; i < source.size(); i++) {
		result += source[i] * item[i];
	}

	return result;
}

Number MathUtils::modulusList(QList<Number> list)
{
	Number result = 0;

	foreach (Number element, list) {
		result += qPow(element, 2);
	}

	return qSqrt(result);
}

QList<Number> MathUtils::quotientListNumber(QList<Number> source, Number divisor)
{
	QList<Number> result;

	foreach (Number element, source) {
		result << element / divisor;
	}

	return result;
}

Number MathUtils::countDeterminant(QVector<QVector<Number> > matrix)
{
	Number result = 0;

	if (matrix.size() == 1) {
		result = matrix[0][0];
	}
	else if (matrix.size() == 2) {
		result = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
	}
	else {
		// Go threw first line
		for (int i = 0; i < matrix.size(); i++) {
			// Initialize minor matrix
			QVector<QVector<Number> > minorMatrix;
			minorMatrix.resize(matrix.size() - 1);
			for (int j = 0; j < minorMatrix.size(); j++) {
				minorMatrix[j].resize(matrix.size() - 1);
			}

			// Fill minor matrix
			for (int j = 1; j < matrix.size(); j++) {
				int k = 0;
				for (int l = 0; l < matrix.size(); l++) {
					// Don't copy the minor column
					if (l != i) {
						minorMatrix[j - 1][k] = matrix[j][l];
						k++;
					}
				}
			}

			result +=
				qPow(-1, 1 + i + 1) *
				matrix[0][i] *
				countDeterminant(minorMatrix);
		}
	}

	return result;
}
