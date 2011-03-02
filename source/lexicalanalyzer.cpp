#include "lexicalanalyzer.h"

QString LexicalAnalyzer::getLexeme()
{

	if (m_lexemeList.isEmpty()) {
		return QString();
	}
	QString tempLexeme = m_lexemeList.first();
	if (tempLexeme.isNull()) {
		return QString();
	}
	else {
		return tempLexeme;
	}

}

void LexicalAnalyzer::nextLexeme()
{
	m_lexemeList.removeFirst();
}

void LexicalAnalyzer::parseInput()
{
	if (m_input.isEmpty()) {
		return;
	}

	int inputLength = m_input.size() - 1;
	int position = 0;

	while (position <= inputLength) {
		int startPosition = position;

		skipSymbols(startPosition, position);

		if (!CheckSymbol::isDigit(m_input.at(position))) {
			return;
		}

		while ( (position < inputLength) && (CheckSymbol::isDigit(m_input.at(position))) ) {
			position++;
		}

		if ( (position < inputLength) && (CheckSymbol::isSeparator(m_input.at(position))) ) {
			position++;
			if ( (position > inputLength) || (!CheckSymbol::isDigit(m_input.at(position))) ) {
				pushLexeme(startPosition, position);
			}
			while ( (position < inputLength) && (CheckSymbol::isDigit(m_input.at(position))) ) {
				position++;
			}
		}

		pushLexeme(startPosition, position);
	}

	addEnd();
}

void LexicalAnalyzer::skipSymbols(int &startPosition, int &position)
{
	while (position < m_input.size() - 1) {
		if (CheckSymbol::isSpace(m_input.at(position))) {
			position++;
			startPosition++;
		}
		else {
			return;
		}
	}
}

void LexicalAnalyzer::pushLexeme(int startPosition, int endPosition)
{
	QString tempNumber;

	while (startPosition < endPosition) {
		tempNumber.append(m_input.at(startPosition));
		startPosition++;
	}

	if (!tempNumber.isNull()) {
		m_lexemeList.append(tempNumber);
	}
}

void LexicalAnalyzer::addEnd()
{
	QString end;
	m_lexemeList.append(end);
}

// Methods below are for checking symbols

bool CheckSymbol::isSeparator(QChar symbol)
{
	QList<QChar> separators;
	separators << '.';
	return separators.contains(symbol);
}

bool CheckSymbol::isSpace(QChar symbol)
{
	QList<QChar> spaces;
	spaces << ' ';
	return spaces.contains(symbol);
}

bool CheckSymbol::isExponent(QChar symbol)
{
	QList<QChar> exponents;
	exponents << 'e' << 'E';
	return exponents.contains(symbol);
}

bool CheckSymbol::isOperation(QChar symbol)
{
	QList<QChar> operations;
	operations << '+' << '-' << '*' << '/';
	return operations.contains(symbol);
}

bool CheckSymbol::isSign(QChar symbol)
{
	QList<QChar> signs;
	signs << '+' << '-';
	return signs.contains(symbol);
}

bool CheckSymbol::isDigit(QChar symbol)
{
	QList<QChar> digits;
	digits << '1' << '2' << '3' << '4' << '5' << '6' << '7' << '8' << '9' << '0';
	return digits.contains(symbol);
}
