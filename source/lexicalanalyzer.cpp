#include "lexicalanalyzer.h"

QString LexicalAnalyzer::getLexeme()
{
	if (m_lexemeList.isEmpty()) {
		return QString();
	}
	lexeme tempLexeme = m_lexemeList.first();
	if (tempLexeme.lexemeData.isNull()) {
		return QString();
	}
	else {
		return tempLexeme.lexemeData;
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
	m_position = 0;

	while (m_position <= inputLength) {
		int startPosition = m_position;
		skipSymbols(startPosition);
		extractLexeme(startPosition);
	}

	addEnd();
}

void LexicalAnalyzer::extractLexeme(int startPosition)
{
	int lexemeType;
	if (CheckSymbol::isDigit(m_input.at(m_position))) {
		lexemeType = 1;
	}
	else {
		m_position = m_input.size();
		return;
	}

	switch (lexemeType) {
		case 1: { extractNumber(startPosition); return;}
		default: return;
	}
}

void LexicalAnalyzer::extractNumber(int &startPosition)
{
	int inputLength = m_input.size() - 1;

	while ( (m_position < inputLength) && (CheckSymbol::isDigit(m_input.at(m_position))) ) {
		m_position++;
	}

	if ( (m_position < inputLength) && (CheckSymbol::isSeparator(m_input.at(m_position))) ) {
		m_position++;
		if ( (m_position > inputLength) || (!CheckSymbol::isDigit(m_input.at(m_position))) ) {
			pushNumber(startPosition);
		}
		while ( (m_position < inputLength) && (CheckSymbol::isDigit(m_input.at(m_position))) ) {
			m_position++;
		}
	}

	pushNumber(startPosition);
}

void LexicalAnalyzer::skipSymbols(int &startPosition)
{
	while (m_position < m_input.size() - 1) {
		if (CheckSymbol::isSpace(m_input.at(m_position))) {
			m_position++;
			startPosition++;
		}
		else {
			return;
		}
	}
}

void LexicalAnalyzer::pushNumber(int startPosition)
{

	QString tempNumber;

	while (startPosition < m_position) {
		tempNumber.append(m_input.at(startPosition));
		startPosition++;
	}

	if (!tempNumber.isNull()) {
		QString lexemeType = "number";
		lexeme tempLexeme;
		tempLexeme.lexemeType = lexemeType;
		tempLexeme.lexemeData = tempNumber;
		m_lexemeList.append(tempLexeme);
	}
}

void LexicalAnalyzer::addEnd()
{
	lexeme tempLexeme;
	QString end;
	tempLexeme.lexemeType = tempLexeme.lexemeData = end;
	m_lexemeList.append(tempLexeme);
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
