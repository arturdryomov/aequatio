#include "lexicalanalyzer.h"

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
	m_input = input;
	
	if (m_input.isEmpty()) {
		throw Exception(tr("Input is empty"));
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
		throw Exception(tr("Lexeme list is empty"));
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
		throw Exception(tr("Lexeme type is not supported"));
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
		throw Exception(tr("Operation is not supported"));
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
		throw Exception(tr("Bracket type is not supported"));
	}

	pushLexeme(lexemeType, QString());
	m_position++;
}

void LexicalAnalyzer::extractPower()
{
	QString operation = m_input.mid(m_position, 1);
	LexemeType lexemeType;
	if (operation == "^") {
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
	QString operation = m_input.mid(m_position, 1);
	LexemeType lexemeType;
	if (operation == "=") {
		lexemeType = LexemeEqual;
	}
	else {
		throw Exception(tr("Equal type is not supported"));
	}

	pushLexeme(lexemeType, QString());
	m_position++;
}

void LexicalAnalyzer::extractComma()
{
	QString operation = m_input.mid(m_position, 1);
	LexemeType lexemeType;
	if (operation == ",") {
		lexemeType = LexemeComma;
	}
	else {
		throw Exception(tr("Comma type is not supported"));
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
	if ( isMinus(c) || isPlus(c) || isMultiply(c) || isDivide(c) || isPower(c) ) {
		return true;
	}
	else {
		return false;
	}
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

bool CheckChar::isComma(QChar c)
{
	QList<QChar> commas;
	commas << ',';
	return commas.contains(c);
}

bool CheckChar::isPlus(QChar c)
{
	QList<QChar> pluses;
	pluses << '+';
	return pluses.contains(c);
}

bool CheckChar::isMinus(QChar c)
{
	// Thanks goes to MSVC++
	QString minuses = "- – − —";
	return minuses.contains(c);
}

bool CheckChar::isMultiply(QChar c)
{
	QString multiplies = "* ×";
	return multiplies.contains(c);
}

bool CheckChar::isDivide(QChar c)
{
	QString divides = "/ ÷";
	return divides.contains(c);
}
