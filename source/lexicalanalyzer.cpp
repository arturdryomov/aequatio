#include "lexicalanalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(QObject *parent) :
	QObject(parent)	
{	
}


Lexeme LexicalAnalyzer::lexeme()
{
	if (m_lexemeList.isEmpty()) {
		throw Exception(tr("Lexeme list is empty"));
	}

	return m_lexemeList.first();
}

void LexicalAnalyzer::nextLexeme()
{
	if (m_lexemeList.first().type != LexemeEOL)
		m_lexemeList.removeFirst();
}

void LexicalAnalyzer::parse(const QString &input)
{
	m_lexemeList.clear();
	m_input = input;
	
	if (m_input.isEmpty()) {
		throw Exception(tr("Input is empty"));
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
	if (CheckChar::isDigit(m_input.at(m_position))) {
		extractNumber();
	}
	else {
		if (CheckChar::isOperation(m_input.at(m_position))) {
			extractOperation();
		}
		else {
			throw Exception(tr("Lexeme type is not supported"));
		}
	}
}

void LexicalAnalyzer::extractNumber()
{
	int inputLength = m_input.size() - 1;
	int startPosition = m_position;

	while ( (m_position <= inputLength) && (CheckChar::isDigit(m_input.at(m_position))) ) {
		m_position++;
	}

	if ( (m_position < inputLength) && (CheckChar::isSeparator(m_input.at(m_position))) ) {
		m_position++;
		if (!( (m_position > inputLength) || (!CheckChar::isDigit(m_input.at(m_position))) ) ) {
			while ( (m_position <= inputLength) && (CheckChar::isDigit(m_input.at(m_position))) ) {
				m_position++;
			}
		}
	}

	QString tempNumber = m_input.mid(startPosition, m_position - startPosition);

	if (!tempNumber.isNull()) {
		LexemeType lexemeType = LexemeNumber;
		pushLexeme(lexemeType, tempNumber);
	}
}

void LexicalAnalyzer::extractOperation()
{
	QString tempOperation = m_input.mid(m_position, 1);
	LexemeType lexemeType;
	if (tempOperation == "+") {
		lexemeType = LexemePlus;
	}
	else if (tempOperation == "-") {
		lexemeType = LexemeMinus;
	}
	else if (tempOperation == "*") {
		lexemeType = LexemeMultiply;
	}
	else if (tempOperation == "/") {
		lexemeType = LexemeDivide;
	}
	else {
		throw Exception(tr("Operation is not supported"));
	}

	pushLexeme(lexemeType, QString());
	m_position++;
}

void LexicalAnalyzer::pushLexeme(LexemeType lexemeType, QString lexemeData)
{
	Lexeme tempLexeme;
	tempLexeme.type = lexemeType;
	tempLexeme.value = lexemeData;
	m_lexemeList.append(tempLexeme);
}

void LexicalAnalyzer::skipWhitespace()
{
	while (m_position < m_input.size() - 1) {
		if (CheckChar::isSpace(m_input.at(m_position))) {
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
	tempLexeme.type = LexemeEOL;
	m_lexemeList.append(tempLexeme);
}

// Methods below are for checking symbols

bool CheckChar::isSeparator(QChar c)
{
	QList<QChar> separators;
	separators << '.';
	return separators.contains(c);
}

bool CheckChar::isSpace(QChar c)
{
	QList<QChar> spaces;
	spaces << ' ';
	return spaces.contains(c);
}

bool CheckChar::isExponent(QChar c)
{
	QList<QChar> exponents;
	exponents << 'e' << 'E';
	return exponents.contains(c);
}

bool CheckChar::isOperation(QChar c)
{
	QList<QChar> operations;
	operations << '+' << '-' << '*' << '/';
	return operations.contains(c);
}

bool CheckChar::isSign(QChar c)
{
	QList<QChar> signs;
	signs << '+' << '-';
	return signs.contains(c);
}

bool CheckChar::isDigit(QChar c)
{
	QList<QChar> digits;
	digits << '1' << '2' << '3' << '4' << '5' << '6' << '7' << '8' << '9' << '0';
	return digits.contains(c);
}

