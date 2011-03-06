#include "syntaxanalyzer.h"
#include "lexicalanalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(QObject *parent) :
	QObject(parent), m_rpnCode(new RpnCode)
{
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
	delete m_rpnCode;
}

QString SyntaxAnalyzer::process(const QString &input)
{
	m_lexicalAnalyzer = new LexicalAnalyzer(input);	
	QString result = "";
	
	if (m_lexicalAnalyzer->lexeme().type == LexemeEOL) {
		throw Exception(tr("Input is empty"));
	}
	
	expression(); // convert expression to RPN
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeEOL) {
		throw Exception(tr("Factor operator or end of file expected"));
	}
	
	
	delete m_lexicalAnalyzer;
	return result;
}

// Expression = Factor {FactorOp Factor}
void SyntaxAnalyzer::expression()
{		
	// note, that exception will be thrown in factor() or factorOp() if something's wrong
	
	// first obligatory factor
	RpnElement operand = factor(); 
	m_rpnCode->elements << operand;
	
	// {FactorOp Factor} section
	while (CheckLexeme::isFactorOp(m_lexicalAnalyzer->lexeme())) {
		RpnElement operation = factorOp();
		operand = factor();
		
		m_rpnCode->elements << operand << operation;
	}		
}

// Factor = Number
RpnElement SyntaxAnalyzer::factor()
{
	if (m_lexicalAnalyzer->lexeme().type != LexemeNumber) {
		throw Exception(tr("Number expected"));
	}
	
	RpnElement result;
	result.type = RpnOperand;
	
	// try to convert
	bool ok = false;	
	result.value = m_lexicalAnalyzer->lexeme().value.toDouble(&ok);
	if (!ok) {
		throw Exception(tr("Cannot convert ‘%1’ to a number")
			.arg(m_lexicalAnalyzer->lexeme().value));
	}
	
	m_lexicalAnalyzer->nextLexeme();
	
	return result;
}

// FactorOp = '*' | '/'
RpnElement SyntaxAnalyzer::factorOp()
{	
	if (CheckLexeme::isFactorOp(m_lexicalAnalyzer->lexeme())) {
		RpnElement result;
		result.type = RpnOperation;
		result.value = m_lexicalAnalyzer->lexeme().type;
		
		m_lexicalAnalyzer->nextLexeme();
		return result;
	}
	else {
		throw Exception(tr("Factoring operator expected"));
	}
}

bool CheckLexeme::isFactorOp(Lexeme lexeme)
{
	return ((lexeme.type == LexemeMultiply) || (lexeme.type == LexemeDivide));
}


