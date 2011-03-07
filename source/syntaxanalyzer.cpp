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
	
	expression(); // convert expression to RPN	
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeEOL) {
		throw Exception(tr("Factor operator or end of file expected"));
	}
	
	// calculate
	return QString::number(m_exprCalculator->calculate(m_rpnCode));		

}

// Expression = Factor {MultOperation Factor}
void SyntaxAnalyzer::expression()
{		
	// note, that exception will be thrown in factor() or multOperation() if something's wrong
	
	// first obligatory factor
	RpnCodeThread operand = factor(); 
	m_rpnCode->elements << operand;
	
	// {MultOperation Factor} section
	while (CheckLexeme::isMultOperation(m_lexicalAnalyzer->lexeme())) {
		RpnElement operation = multOperation();
		operand = factor();
		
		m_rpnCode->elements << operand << operation;
	}		
}

// Factor = Number
RpnCodeThread SyntaxAnalyzer::factor()
{
	if (m_lexicalAnalyzer->lexeme().type != LexemeNumber) {
		throw Exception(tr("Number expected"));
	}
	
	// try to convert
	bool ok = false;	
	Number value = m_lexicalAnalyzer->lexeme().value.toDouble(&ok);
	if (!ok) {
		throw Exception(tr("Cannot convert ‘%1’ to a number")
			.arg(m_lexicalAnalyzer->lexeme().value));
	}
	
	RpnElement element = {RpnOperand, value};
	
	RpnCodeThread result;
	result.append(element);
	
	m_lexicalAnalyzer->nextLexeme();
	
	return result;
}

// MultOperation = '*' | '/'
RpnElement SyntaxAnalyzer::multOperation()
{	
	RpnElement result;

	if (m_lexicalAnalyzer->lexeme().type == LexemeMultiply) {
		result.type = RpnOperation;
		result.value.setValue(OperationMultiply);
	}
	else if(m_lexicalAnalyzer->lexeme().type == LexemeDivide) {
		result.type = RpnOperation;
		result.value.setValue(OperationDivide);
	}
	else {
		throw Exception(tr("Factoring operator expected"));
	}

	m_lexicalAnalyzer->nextLexeme();
	return result;
}

bool CheckLexeme::isMultOperation(Lexeme lexeme)
{
	return ((lexeme.type == LexemeMultiply) || (lexeme.type == LexemeDivide));
}