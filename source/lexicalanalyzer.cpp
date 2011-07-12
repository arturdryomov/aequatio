#include "lexicalanalyzer.h"
#include "parsingexceptions.h"

LexicalAnalyzer::LexicalAnalyzer(QObject *parent) :
	QObject(parent),
	m_lexemeListIterator(0)
{
	initializeReservedWords();
}

LexicalAnalyzer::~LexicalAnalyzer()
{
	if (m_lexemeListIterator != 0) {
		delete m_lexemeListIterator;
	}
}

Lexeme LexicalAnalyzer::lexeme()
{
	if (m_lexemeListIterator->hasNext()) {
		return m_lexemeListIterator->peekNext();
	}
	else {
		return EndLexeme();
	}
}

void LexicalAnalyzer::nextLexeme()
{
	if (m_lexemeListIterator->hasNext()) {
		m_lexemeListIterator->next();
	}		
}

void LexicalAnalyzer::previousLexeme()
{
	if (m_lexemeListIterator->hasPrevious()) {
		m_lexemeListIterator->previous();
	}	
}

void LexicalAnalyzer::parse(const QString &input)
{
	m_lexemeList.clear();
	m_input = input.trimmed();
	
	if (m_input.isEmpty()) {
		THROW(EEmptyInput());
	}

	m_position = 0;
	forever {
		skipWhitespace();

		// Loop exit condition
		if (m_position >= m_input.size()) {
			break;
		}

		extractLexeme();
	}
	
	// ensure there are lexemes
	if (m_lexemeList.isEmpty()) {
		THROW(EInternal());
	}	
	
	// create new constant iterator for lexemes
	if (m_lexemeListIterator != 0) {
		delete m_lexemeListIterator;
	}
	m_lexemeListIterator = new QListIterator<Lexeme>(m_lexemeList);
}

void LexicalAnalyzer::initializeReservedWords()
{
	m_reservedWords.insert("const", LexemeConst);
	m_reservedWords.insert("func", LexemeFunc);
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
	else if (CheckChar::isComma(m_input.at(m_position))) {
		extractComma();
	}
	else if (CheckChar::isLetterOrUnderscore(m_input.at(m_position))) {
		extractIdentifier();
	}
	else {
		THROW(EUnsupportedLexeme("Lexeme"));
	}
}

void LexicalAnalyzer::extractIdentifier()
{
	int inputLength = m_input.size() - 1;
	int startPosition = m_position;

	while ( (m_position <= inputLength) &&
		( (CheckChar::isDigit(m_input.at(m_position))) ||
		(CheckChar::isLetterOrUnderscore(m_input.at(m_position))) ) ) {
		m_position++;
	}

	QString identifier = m_input.mid(startPosition, m_position - startPosition);

	if (!identifier.isNull()) {
		if (m_reservedWords.contains(identifier)) {
			LexemeType lexemeType = m_reservedWords.value(identifier);
			pushLexeme(lexemeType, QString());
		}
		else {
			LexemeType lexemeType = LexemeIdentifier;
			pushLexeme(lexemeType, identifier);
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
			THROW(ELexemeExpected("Number after dot"))
		}
		while ( (m_position <= inputLength) && (CheckChar::isDigit(m_input.at(m_position))) ) {
			m_position++;
		}
	}

	if ( (m_position <= inputLength) && (CheckChar::isExponent(m_input.at(m_position))) ) {
		m_position++;
		if (m_position > inputLength) {
			THROW(ELexemeExpected("Number or sign after exponent character"))
		}
		if (CheckChar::isSign(m_input.at(m_position))) {
			m_position++;
		}
		if ( (m_position > inputLength) || (!CheckChar::isDigit(m_input.at(m_position))) ) {
			THROW(ELexemeExpected("Number or sign after exponent character"))
		}
		while ( (m_position <= inputLength) && (CheckChar::isDigit(m_input.at(m_position))) ) {
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
	QChar operation = m_input.at(m_position);
	LexemeType lexemeType;
	if (CheckChar::isPlus(operation)) {
		lexemeType = LexemePlus;
	}
	else if (CheckChar::isMinus(operation)) {
		lexemeType = LexemeMinus;
	}
	else if (CheckChar::isMultiply(operation)) {
		lexemeType = LexemeMultiply;
	}
	else if (CheckChar::isDivide(operation)) {
		lexemeType = LexemeDivide;
	}
	else if (CheckChar::isPower(operation)) {
		lexemeType = LexemePower;
	}
	else {
		THROW(EUnsupportedLexeme("Operation"));
	}

	pushLexeme(lexemeType, QString());
	m_position++;
}

void LexicalAnalyzer::extractBracket()
{
	QString bracket = m_input.mid(m_position, 1);
	LexemeType lexemeType;
	if (bracket == "(") {
		lexemeType = LexemeOpeningBracket;
	}
	else if (bracket == ")") {
		lexemeType = LexemeClosingBracket;
	}
	else {
		THROW(EUnsupportedLexeme("Bracket"));
	}

	pushLexeme(lexemeType, QString());
	m_position++;
}

void LexicalAnalyzer::extractPower()
{
	QChar operation = m_input.at(m_position);
	LexemeType lexemeType;
	if (CheckChar::isPower(operation)) {
		lexemeType = LexemePower;
	}
	else {
		THROW(EUnsupportedLexeme("Power"));
	}

	pushLexeme(lexemeType, QString());
	m_position++;
}

void LexicalAnalyzer::extractEqual()
{
	QChar operation = m_input.at(m_position);
	LexemeType lexemeType;
	if (CheckChar::isEqual(operation)) {
		lexemeType = LexemeEqual;
	}
	else {
		THROW(EUnsupportedLexeme("Equal sign"));
	}

	pushLexeme(lexemeType, QString());
	m_position++;
}

void LexicalAnalyzer::extractComma()
{
	QChar operation = m_input.at(m_position);
	LexemeType lexemeType;
	if (CheckChar::isComma(operation)) {
		lexemeType = LexemeComma;
	}
	else {
		THROW(EUnsupportedLexeme("Comma"));
	}

	pushLexeme(lexemeType, QString());
	m_position++;	
}

void LexicalAnalyzer::pushLexeme(LexemeType lexemeType, QString lexemeData)
{
	Lexeme lexeme;
	lexeme.type = lexemeType;
	lexeme.value = lexemeData;
	m_lexemeList.append(lexeme);
}

void LexicalAnalyzer::skipWhitespace()
{
	while (m_position < m_input.size()) {
		if (CheckChar::isSpace(m_input.at(m_position))) {
			m_position++;
		}
		else {
			return;
		}
	}
}

Lexeme LexicalAnalyzer::EndLexeme()
{
	Lexeme result = {LexemeEol, QString()};
	return result;
}

// Methods below are for checking symbols

bool CheckChar::isSeparator(QChar c)
{
	return c == '.';
}

bool CheckChar::isSpace(QChar c)
{
	return c == ' ';
}

bool CheckChar::isExponent(QChar c)
{
	QString exponents = "eE";
	return exponents.contains(c);
}

bool CheckChar::isOperation(QChar c)
{
	return isMinus(c) || isPlus(c) || isMultiply(c) || isDivide(c) || isPower(c);
}

bool CheckChar::isSign(QChar c)
{
	QString signs = "+-";
	return signs.contains(c);
}

bool CheckChar::isBracket(QChar c)
{
	QString brackets = "()";
	return brackets.contains(c);
}

bool CheckChar::isPower(QChar c)
{
	return c == '^';
}

bool CheckChar::isDigit(QChar c)
{
	QString digits = "1234567890";
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
	return c == '_';
}

bool CheckChar::isEqual(QChar c)
{
	return c == '=';
}

bool CheckChar::isComma(QChar c)
{
	return c == ',';
}

bool CheckChar::isPlus(QChar c)
{
	return c == '+';
}

bool CheckChar::isMinus(QChar c)
{
	// There are no beautiful way to work with unicode char literals
	// GCC knows how to do that task, but MSVC++ isn't so cool
	QString minuses = "-–−—";
	return minuses.contains(c);
}

bool CheckChar::isMultiply(QChar c)
{
	QString multiplies = "*×";
	return multiplies.contains(c);
}

bool CheckChar::isDivide(QChar c)
{
	QString divides = "/÷";
	return divides.contains(c);
}
