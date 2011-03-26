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

void LexicalAnalyzer::initializeReservedWords()
{
	m_reservedWords.insert("const", LexemeConst);
	m_reservedWords.insert("func", LexemeConst);
}

void LexicalAnalyzer::extractLexeme()
{
	if (CheckChar::isDigit(m_input.at(m_position))) {
		extractNumber();
	}
	else if (CheckChar::isOperation(m_input.at(m_position))) {
		extractOperation();
	}
	else if (CheckChar::isBracket(m_input.at(m_position))) {
		extractBracket();
	}
	else if (CheckChar::isPower(m_input.at(m_position))) {
		extractPower();
	}
	else if (CheckChar::isEqual(m_input.at(m_position))) {
		extractEqual();
	}
	else if (CheckChar::isLetterOrUnderscore(m_input.at(m_position))) {
		extractIdentifyer();
	}
	else {
		throw Exception(tr("Lexeme type is not supported"));
	}
}


void LexicalAnalyzer::extractIdentifyer()
{
	int inputLength = m_input.size() - 1;
	int startPosition = m_position;

	while ( (m_position <= inputLength) &&
		( (CheckChar::isDigit(m_input.at(m_position))) ||
			(CheckChar::isLetterOrUnderscore(m_input.at(m_position))) ) ) {
		m_position++;
	}

	QString tempIdentifyer = m_input.mid(startPosition, m_position - startPosition);

	if (!tempIdentifyer.isNull()) {
		if (m_reservedWords.contains(tempIdentifyer)) {
			LexemeType lexemeType = m_reservedWords.value(tempIdentifyer);
			pushLexeme(lexemeType, QString());
		}
		else {
			LexemeType lexemeType = LexemeIdentifyer;
			pushLexeme(lexemeType, tempIdentifyer);
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
		if ( (m_position > inputLength) || (!CheckChar::isDigit(m_input.at(m_position))) )  {
			throw Exception(tr("Must be number after dot"));
		}
		while ( (m_position <= inputLength) && (CheckChar::isDigit(m_input.at(m_position))) ) {
			m_position++;
		}
	}

	if ( (m_position <= inputLength) && (CheckChar::isExponent(m_input.at(m_position))) ) {
		m_position++;
		if (m_position > inputLength) {
			throw Exception(tr("Must be number or sign after exponent sign"));
		}
		if (CheckChar::isSign(m_input.at(m_position))) {
			m_position++;
		}
		if ( (m_position > inputLength) || (!CheckChar::isDigit(m_input.at(m_position))) ) {
			throw Exception(tr("Must be number or sign after exponent sign"));
		}
		while ( (m_position < inputLength) && (CheckChar::isDigit(m_input.at(m_position))) ) {
			m_position++;
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

void LexicalAnalyzer::extractBracket()
{
	QString tempBracket = m_input.mid(m_position, 1);
	LexemeType lexemeType;
	if (tempBracket == "(") {
		lexemeType = LexemeOpeningBracket;
	}
	else if (tempBracket == ")") {
		lexemeType = LexemeClosingBracket;
	}
	else {
		throw Exception(tr("Bracket type is not supported"));
	}

	pushLexeme(lexemeType, QString());
	m_position++;
}

void LexicalAnalyzer::extractPower()
{
	QString tempOperation = m_input.mid(m_position, 1);
	LexemeType lexemeType;
	if (tempOperation == "^") {
		lexemeType = LexemePower;
	}
	else {
		throw Exception(tr("Power type is not supported"));
	}

	pushLexeme(lexemeType, QString());
	m_position++;
}

void LexicalAnalyzer::extractEqual()
{
	QString tempOperation = m_input.mid(m_position, 1);
	LexemeType lexemeType;
	if (tempOperation == "=") {
		lexemeType = LexemeEqual;
	}
	else {
		throw Exception(tr("Equal type is not supported"));
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

bool CheckChar::isBracket(QChar c)
{
	QList<QChar> brackets;
	brackets << '(' << ')';
	return brackets.contains(c);
}

bool CheckChar::isPower(QChar c)
{
	QList<QChar> powers;
	powers << '^';
	return powers.contains(c);
}

bool CheckChar::isDigit(QChar c)
{
	QList<QChar> digits;
	digits << '1' << '2' << '3' << '4' << '5' << '6' << '7' << '8' << '9' << '0';
	return digits.contains(c);
}

bool CheckChar::isLetterOrUnderscore(QChar c)
{
	if ( (CheckChar::isLetter(c)) || (CheckChar::isUnderscore(c)) ) {
		return true;
	}
	else {
		return false;
	}
}

bool CheckChar::isLetter(QChar c)
{
	if ( ( (c >= 'A') && (c <='Z') ) || ( (c >= 'a') && (c <='z') ) ) {
		return true;
	}
	else {
		return false;
	}
}

bool CheckChar::isUnderscore(QChar c)
{
	QList<QChar> underscores;
	underscores << '_';
	return underscores.contains(c);
}

bool CheckChar::isEqual(QChar c)
{
	QList<QChar> equals;
	equals << '=';
	return equals.contains(c);
}
