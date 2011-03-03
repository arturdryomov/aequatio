#include "lexicalanalyzer.h"

QString LexicalAnalyzer::getLexeme()
{
	if (m_lexemeList.isEmpty()) {
		return QString();
	}
	Lexeme tempLexeme = m_lexemeList.first();
	if (tempLexeme.lexemeType == lexemeEOL) {
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
		throw Exception("Input is empty");
	}

	int inputLength = m_input.size() - 1;
	m_position = 0;

	while (m_position <= inputLength) {
		skipSymbols();
		extractLexeme();
	}

	addEnd();
}

void LexicalAnalyzer::extractLexeme()
{
	if (CheckSymbol::isDigit(m_input.at(m_position))) {
		extractNumber();
	}
	else {
		m_position = m_input.size();
		return;
	}
}

void LexicalAnalyzer::extractNumber()
{
	int inputLength = m_input.size() - 1;
	int startPosition = m_position;

	while ( (m_position < inputLength) && (CheckSymbol::isDigit(m_input.at(m_position))) ) {
		m_position++;
	}

	if ( (m_position < inputLength) && (CheckSymbol::isSeparator(m_input.at(m_position))) ) {
		m_position++;
		if (!( (m_position > inputLength) || (!CheckSymbol::isDigit(m_input.at(m_position))) ) ) {
			while ( (m_position < inputLength) && (CheckSymbol::isDigit(m_input.at(m_position))) ) {
				m_position++;
			}
		}
	}

	QString tempNumber;
	while (startPosition < m_position) {
		tempNumber.append(m_input.at(startPosition));
		startPosition++;
	}

	if (!tempNumber.isNull()) {
		LexemeType lexemeType = lexemeNumber;
		pushLexeme(lexemeType, tempNumber);
	}
}

void LexicalAnalyzer::pushLexeme(LexemeType lexemeType, QString lexemeData)
{
	Lexeme tempLexeme;
	tempLexeme.lexemeType = lexemeType;
	tempLexeme.lexemeData = lexemeData;
	m_lexemeList.append(tempLexeme);
}

void LexicalAnalyzer::skipSymbols()
{
	while (m_position < m_input.size() - 1) {
		if (CheckSymbol::isSpace(m_input.at(m_position))) {
			m_position++;
		}
		else {
			return;
		}
	}
}

void LexicalAnalyzer::addEnd()
{
	Lexeme tempLexeme;
	tempLexeme.lexemeType = lexemeEOL;
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
