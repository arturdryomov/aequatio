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
	if (m_lexicalAnalyzer->lexeme().type == LexemeEOL) {
		throw Exception(tr("Input is empty"));
	}
	
	m_rpnCode->elements << expression(); // convert expression to RPN	
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeEOL) {
		throw Exception(tr("End of file expected"));
	}
	
	// calculate
	return QString::number(m_exprCalculator->calculate(m_rpnCode));		

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

// Factor = Number | '('Expression')'
RpnCodeThread SyntaxAnalyzer::factor()
{
	RpnCodeThread result;
	
	// Number
	if (m_lexicalAnalyzer->lexeme().type == LexemeNumber) {
		
		// try to convert
		bool ok = false;	
		Number value = m_lexicalAnalyzer->lexeme().value.toDouble(&ok);
		if (!ok) {
			throw Exception(tr("Cannot convert ‘%1’ to a number")
				.arg(m_lexicalAnalyzer->lexeme().value));
		}
		
		RpnElement element = {RpnOperand, value};		
		result << element;		
		m_lexicalAnalyzer->nextLexeme();		
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

bool CheckLexeme::isMultOperation(Lexeme lexeme)
{
	return ((lexeme.type == LexemeMultiply) || (lexeme.type == LexemeDivide));
}

bool CheckLexeme::isSummOperation(Lexeme lexeme)
{
	return ((lexeme.type == LexemePlus) || (lexeme.type == LexemeMinus));
}

