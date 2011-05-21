#include "syntaxanalyzer.h"
#include "lexicalanalyzer.h"
#include <QMetaType>

SyntaxAnalyzer::SyntaxAnalyzer(QObject *parent) :
	QObject(parent), 	
	m_rpnCode(new RpnCode),
	m_lexicalAnalyzer(new LexicalAnalyzer),
	m_exprCalculator(new ExprCalculator(this))
{
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
	delete m_rpnCode;
}

QString SyntaxAnalyzer::process(const QString &input)
{	
	m_rpnCode->elements.clear();
	
	// perform lexical analyzis
	m_lexicalAnalyzer->parse(input);		
	if (m_lexicalAnalyzer->lexeme().type == LexemeEol) {
		throw Exception(tr("Input is empty"));
	}
	
	// Process the command: const declaration or expression
	return command();
}

// Command = ConstDeclaration | Expression
QString SyntaxAnalyzer::command()
{
	QString result;
	
	// const declaration
	if (m_lexicalAnalyzer->lexeme().type == LexemeConst) {
		result = constDeclaration(); // result here is just a notification
		ensureNoMoreLexemes();	
	}
	
	// expression
	else {
		m_rpnCode->elements << expression(); // convert expression to RPN
		ensureNoMoreLexemes();		
		result = QString::number(m_exprCalculator->calculate(m_rpnCode)); // calculate
	}
	
	return result;	
}

// ConstDeclaration = 'const' Identifier '=' {Unary Op} Number
QString SyntaxAnalyzer::constDeclaration()
{
	// 'const'
	if (m_lexicalAnalyzer->lexeme().type != LexemeConst) {
		throw Exception(tr("Illegal constant declaration beginning"));
	}
	m_lexicalAnalyzer->nextLexeme();
	
	// Identifier
	if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
		throw Exception(tr("Identifier after ‘const’ expected"));
	}	
	QString constName = m_lexicalAnalyzer->lexeme().value;
	m_lexicalAnalyzer->nextLexeme();
	
	// '='
	if (m_lexicalAnalyzer->lexeme().type != LexemeEqual) {
		throw Exception(tr("Sign ‘=’ expected after identifier"));
	}
	m_lexicalAnalyzer->nextLexeme();
	
	// {UnaryOp}
	int signMultiplyer = 1;
	while (CheckLexeme::isUnaryOperation(m_lexicalAnalyzer->lexeme())) {
		if (m_lexicalAnalyzer->lexeme().type == LexemeMinus) {
			signMultiplyer *= -1;
		}
		m_lexicalAnalyzer->nextLexeme();
	}
	
	// Number
	if (m_lexicalAnalyzer->lexeme().type != LexemeNumber) {
		throw Exception(tr("Number after ‘=’ expected"));
	}	
	Number constValue = number();
	m_lexicalAnalyzer->nextLexeme();
	
	// add constant to list
	m_consts[constName] = constValue;
	
	// return notification
	return tr("Constant ‘%1’ now means ‘%2’").arg(constName).arg(constValue);
}

// Expression = Summand {SummOperator Summand}
RpnCodeThread SyntaxAnalyzer::expression()
{		
	/* Note, that exception will be thrown in factor(), multOperation(), etc if something's wrong. */
	
	RpnCodeThread result;
	
	// first obligatory summand		
	RpnCodeThread operand = summand(); 
	result << operand;	
	
	// {SummOperator Summand} section
	while (CheckLexeme::isSummOperation(m_lexicalAnalyzer->lexeme())) {
		RpnElement operation = summOperation();
		operand = summand();
		
		result << operand << operation;
	}	
	
	return result;		
}

// Factor = (UnaryOp Factor) | (PowerBase ['^' Factor]) 
RpnCodeThread SyntaxAnalyzer::factor()
{
	RpnCodeThread result;
	
	// UnaryOp Factor
	if (CheckLexeme::isUnaryOperation(m_lexicalAnalyzer->lexeme())) {
		
		if (m_lexicalAnalyzer->lexeme().type == LexemeMinus) {			
			
			m_lexicalAnalyzer->nextLexeme();
			
			// -A = (-1) * A
			RpnElement minusOne = {RpnOperand, -1};
			RpnElement multiply = {RpnOperation, QVariant::fromValue(OperationMultiply)}; 
			RpnCodeThread rightOperand = factor();
			
			result << minusOne << rightOperand << multiply;
		}
		
		else if (m_lexicalAnalyzer->lexeme().type == LexemePlus) {			
			m_lexicalAnalyzer->nextLexeme();
			result = factor();
		}
		
		else {
			throw Exception(tr("Unsupported unary operation"));
		}
	}
	
	else {
		RpnCodeThread base = powerBase();
		result << base;
		
		// ['^' Factor]
		if (m_lexicalAnalyzer->lexeme().type == LexemePower) {
			RpnElement power = {RpnOperation, QVariant::fromValue(OperationPower)};
			m_lexicalAnalyzer->nextLexeme();
			RpnCodeThread exponent = factor();
			
			result << exponent << power;
		}
		
	}
	
	return result;
}

// PowerBase = Number | Constant | '('Expression')'
RpnCodeThread SyntaxAnalyzer::powerBase()
{
	RpnCodeThread result;
	
	// Number
	if (m_lexicalAnalyzer->lexeme().type == LexemeNumber) {		
		Number value = number();		
		RpnElement element = {RpnOperand, value};		
		result << element;					
	}
	
	// Constant
	else if (m_lexicalAnalyzer->lexeme().type == LexemeIdentifier) {
		Number value = constant();
		RpnElement element = {RpnOperand, value};		
		result << element;	
	}
	
	// '('Expression')'
	else if (m_lexicalAnalyzer->lexeme().type == LexemeOpeningBracket) {
		
		m_lexicalAnalyzer->nextLexeme();		
		result = expression();
		
		if (m_lexicalAnalyzer->lexeme().type != LexemeClosingBracket) {
			throw Exception(tr("Closing bracket expected"));
		}
		
		m_lexicalAnalyzer->nextLexeme();
	}
	
	else {
		throw Exception(tr("Number or expression in brackets expected"));
	}	

	return result;	
}

// MultOperation = '*' | '/'
RpnElement SyntaxAnalyzer::multOperation()
{	
	RpnElement result;
	result.type = RpnOperation;
	
	if (m_lexicalAnalyzer->lexeme().type == LexemeMultiply) {		
		result.value.setValue(OperationMultiply);
	}
	else if(m_lexicalAnalyzer->lexeme().type == LexemeDivide) {
		result.value.setValue(OperationDivide);
	}
	else {
		throw Exception(tr("Multiplication operator expected"));
	}

	m_lexicalAnalyzer->nextLexeme();
	return result;
}

// Summand = Factor {MultOperator Factor}
RpnCodeThread SyntaxAnalyzer::summand()
{
	RpnCodeThread result;
	// first obligatory factor
	RpnCodeThread operand = factor(); 
	result << operand;
	
	// {MultOperation Factor} section
	while (CheckLexeme::isMultOperation(m_lexicalAnalyzer->lexeme())) {
		RpnElement operation = multOperation();
		operand = factor();

		result << operand << operation;
	}

	return result;
}

// SummOperation = '+' | '-'
RpnElement SyntaxAnalyzer::summOperation()
{
	RpnElement result;
	result.type = RpnOperation;
	
	if (m_lexicalAnalyzer->lexeme().type == LexemePlus) {		
		result.value.setValue(OperationPlus);
	}
	else if(m_lexicalAnalyzer->lexeme().type == LexemeMinus) {
		result.value.setValue(OperationMinus);
	}
	else {
		throw Exception(tr("Summation operator expected"));
	}

	m_lexicalAnalyzer->nextLexeme();
	return result;	
}

Number SyntaxAnalyzer::number()
{
	bool ok = false;	
	Number result = m_lexicalAnalyzer->lexeme().value.toDouble(&ok);
	if (!ok) {
		throw Exception(tr("Cannot convert ‘%1’ to a number").arg(m_lexicalAnalyzer->lexeme().value));
	}
	m_lexicalAnalyzer->nextLexeme();	
	return result;
}

// Constant = Identifier
Number SyntaxAnalyzer::constant()
{
	QString constName = m_lexicalAnalyzer->lexeme().value;
	if (!m_consts.contains(constName)) {
		throw Exception(tr("Undeclared identifier ‘%1’").arg(constName));
	}
	m_lexicalAnalyzer->nextLexeme();
	
	return m_consts.value(constName);	
}

void SyntaxAnalyzer::ensureNoMoreLexemes()
{
	if (m_lexicalAnalyzer->lexeme().type != LexemeEol) {
	   throw Exception(tr("End of file expected"));
   }
}

bool CheckLexeme::isMultOperation(Lexeme lexeme)
{
	return ((lexeme.type == LexemeMultiply) || (lexeme.type == LexemeDivide));
}

bool CheckLexeme::isSummOperation(Lexeme lexeme)
{
	return ((lexeme.type == LexemePlus) || (lexeme.type == LexemeMinus));
}

bool CheckLexeme::isUnaryOperation(Lexeme lexeme)
{
	return ((lexeme.type == LexemePlus) || (lexeme.type == LexemeMinus));
}