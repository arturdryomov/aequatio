#include "lexicalanalyzer.h"

Lexeme LexicalAnalyzer::getLexeme()
{
	if (m_lexemeList.isEmpty()) {
		throw Exception("Lexeme list is empty");
	}

	return m_lexemeList.first();
}

void LexicalAnalyzer::nextLexeme()
{
	if (m_lexemeList.first().lexemeType != lexemeEOL)
		m_lexemeList.removeFirst();
}

void LexicalAnalyzer::parseInput()
{
	if (m_input.isEmpty()) {
		throw Exception("Input is empty");
	}

	m_position = 0;
	while (m_position < m_input.size()) {
		skipWhitespace();
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
		if (CheckSymbol::isOperation(m_input.at(m_position))) {
			extractOperation();
		}
		else {
			throw Exception("Lexeme type does not support");
		}
	}
}

void LexicalAnalyzer::extractNumber()
{
	int inputLength = m_input.size() - 1;
	int startPosition = m_position;

	while ( (m_position <= inputLength) && (CheckSymbol::isDigit(m_input.at(m_position))) ) {
		m_position++;
	}

	if ( (m_position < inputLength) && (CheckSymbol::isSeparator(m_input.at(m_position))) ) {
		m_position++;
		if (!( (m_position > inputLength) || (!CheckSymbol::isDigit(m_input.at(m_position))) ) ) {
			while ( (m_position <= inputLength) && (CheckSymbol::isDigit(m_input.at(m_position))) ) {
				m_position++;
			}
		}
	}

	QString tempNumber = m_input.mid(startPosition, m_position - startPosition);

	if (!tempNumber.isNull()) {
		LexemeType lexemeType = lexemeNumber;
		pushLexeme(lexemeType, tempNumber);
	}
}

void LexicalAnalyzer::extractOperation()
{
	QString tempOperation = m_input.mid(m_position, 1);
	if (!tempOperation.isNull()) {
		LexemeType lexemeType = lexemeOperation;
		pushLexeme(lexemeType, tempOperation);
	}

	m_position++;
}

void LexicalAnalyzer::pushLexeme(LexemeType lexemeType, QString lexemeData)
{
	Lexeme tempLexeme;
	tempLexeme.lexemeType = lexemeType;
	tempLexeme.lexemeData = lexemeData;
	m_lexemeList.append(tempLexeme);
}

void LexicalAnalyzer::skipWhitespace()
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

bool CheckSymbol::isSeparator(QChar c)
{
	QList<QChar> separators;
	separators << '.';
	return separators.contains(c);
}

bool CheckSymbol::isSpace(QChar c)
{
	QList<QChar> spaces;
	spaces << ' ';
	return spaces.contains(c);
}

bool CheckSymbol::isExponent(QChar c)
{
	QList<QChar> exponents;
	exponents << 'e' << 'E';
	return exponents.contains(c);
}

bool CheckSymbol::isOperation(QChar c)
{
	QList<QChar> operations;
	operations << '+' << '-' << '*' << '/';
	return operations.contains(c);
}

bool CheckSymbol::isSign(QChar c)
{
	QList<QChar> signs;
	signs << '+' << '-';
	return signs.contains(c);
}

bool CheckSymbol::isDigit(QChar c)
{
	QList<QChar> digits;
	digits << '1' << '2' << '3' << '4' << '5' << '6' << '7' << '8' << '9' << '0';
	return digits.contains(c);
}
