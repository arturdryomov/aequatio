#include "syntaxanalyzer.h"
#include "lexicalanalyzer.h"
#include <QMetaType>

SyntaxAnalyzer::SyntaxAnalyzer(QObject *parent) :
	QObject(parent),
	m_lexicalAnalyzer(new LexicalAnalyzer),
	m_exprCalculator(new ExprCalculator(this))
{
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
}

QString SyntaxAnalyzer::process(const QString &input)
{
	// there will be new Main function
	m_rpnCode.remove(RpnFunctionMain);

	// perform lexical analyzis
	m_lexicalAnalyzer->parse(input);
	if (m_lexicalAnalyzer->lexeme().type == LexemeEol) {
		throw Exception(tr("Input is empty"));
	}

	// Process the command: const declaration, const function or expression
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

	// func declaration
	else if (m_lexicalAnalyzer->lexeme().type == LexemeFunc) {
		result = functionDeclaration(); // result here is a notification too
		ensureNoMoreLexemes();
	}

	// expression
	else {
		RpnFunction mainFunction;
		mainFunction.argumentsCount = 0;
		mainFunction.codeThread << expression(); // convert expression to RPN
		m_rpnCode.insert(RpnFunctionMain, mainFunction);
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

// FunctionDeclaration = 'func' Indenifier '(' FormalArgument
//		{ ',' FormalArgument} ')' '=' Expression
QString SyntaxAnalyzer::functionDeclaration()
{
	/* Get function name */

	// 'func'
	if (m_lexicalAnalyzer->lexeme().type != LexemeFunc) {
		throw Exception(tr("Illegal function declaration beginning"));
	}
	m_lexicalAnalyzer->nextLexeme();

	// Identifier
	if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
		throw Exception(tr("Identifyer after ‘func’ expected"));
	}
	QString functionName = m_lexicalAnalyzer->lexeme().value;
	m_lexicalAnalyzer->nextLexeme();
	
	
	/* Ensure there's no built in function with this name */
	if (m_exprCalculator->isBuiltInFunction(functionName)) {
		throw Exception(tr("There is built in function named ‘%1’").arg(functionName));
	}
	
	
	/* Get formal arguments and save them to list */
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeOpeningBracket) {
		throw Exception(tr("Opening bracket expected after function name"));
	}
	
	do {
		m_lexicalAnalyzer->nextLexeme();
		extractFormalArgument();
	} while (m_lexicalAnalyzer->lexeme().type == LexemeComma);
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeClosingBracket) {
		throw Exception(tr("Closing bracket expected after formal arguments list"));
	}
	m_lexicalAnalyzer->nextLexeme();
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeEqual) {
		throw Exception(tr("Equal sign expected after closing bracket"));
	}
	m_lexicalAnalyzer->nextLexeme();
	
	/* Parse the function body and save it */
	
	RpnFunction function;
	function.argumentsCount = m_workingParams.count();
	function.codeThread = expression();
	m_workingParams.clear();
	m_rpnCode.insert(functionName, function);
	
	
	/* Parse the function body and save it */
	return tr("Function ‘%1’ is declared").arg(functionName);
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

// Function = Identifier'(' ActualArgument{ ',' ActualArgument}')'
RpnCodeThread SyntaxAnalyzer::function()
{
	RpnCodeThread result;
	
	// Get function name and ensure it is built in or user defined	
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
		throw Exception(tr("Function name expected"));
	}	
	QString functionName = m_lexicalAnalyzer->lexeme().value;		
	int formalArgumentsCount;
	if (m_exprCalculator->isBuiltInFunction(functionName)) {
		formalArgumentsCount = m_exprCalculator->builtInFunctionArgumentsCount(functionName);
	} 
	else if (m_rpnCode.contains(functionName)) {
		formalArgumentsCount = m_rpnCode[functionName].argumentsCount;
	}	
	else {
		throw Exception(tr("Undeclared function ‘%1’ call").arg(functionName));
	}	
	m_lexicalAnalyzer->nextLexeme();	
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeOpeningBracket) {
		throw Exception(tr("Opening bracket expected"));
	}
	
	// Parse actual arguments and add them to tread. 
	// Ensure their count equals formal arguments count
	int actualArgumentsCount = 0;
	do {
		m_lexicalAnalyzer->nextLexeme();
		result << expression();
		actualArgumentsCount++;
		if (actualArgumentsCount > formalArgumentsCount) {
			throw Exception(tr("Too many arguments in ‘%1’ function call").arg(functionName));
		}		
	} while (m_lexicalAnalyzer->lexeme().type == LexemeComma);
	
	if (actualArgumentsCount != formalArgumentsCount) {
		throw Exception(tr("Not enough arguments in ‘%1’ function call").arg(functionName));
	}
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeClosingBracket) {
		throw Exception(tr("Closing bracket expected"));
	}	
	m_lexicalAnalyzer->nextLexeme();
	
	// Add to thread a function call element
	RpnElement functionCall;
	functionCall.type = RpnElementFunction;
	functionCall.value = functionName;
	result << functionCall;
	
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
			RpnElement minusOne = {RpnElementOperand, -1};
			RpnElement multiply = {RpnElementFunction, QVariant::fromValue(RpnFunctionMultiply)};
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
			RpnElement power = {RpnElementFunction, QVariant::fromValue(RpnFunctionPower)};
			m_lexicalAnalyzer->nextLexeme();
			RpnCodeThread exponent = factor();

			result << exponent << power;
		}

	}

	return result;
}

// PowerBase = Number | Constant | Function | '('Expression')'
RpnCodeThread SyntaxAnalyzer::powerBase()
{
	RpnCodeThread result;

	// Number
	if (m_lexicalAnalyzer->lexeme().type == LexemeNumber) {
		Number value = number();
		RpnElement element = {RpnElementOperand, value};
		result << element;
	}

	// Constant | Function
	else if (m_lexicalAnalyzer->lexeme().type == LexemeIdentifier) {
		// Constant
		m_lexicalAnalyzer->nextLexeme();
		if (m_lexicalAnalyzer->lexeme().type != LexemeOpeningBracket) {
			m_lexicalAnalyzer->previousLexeme();
			RpnElement element = constant();
			result << element;
		}
		// Function
		else {
			m_lexicalAnalyzer->previousLexeme();
			RpnCodeThread thread = function();
			result << thread;
		}		
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
	result.type = RpnElementFunction;

	if (m_lexicalAnalyzer->lexeme().type == LexemeMultiply) {
		result.value.setValue(RpnFunctionMultiply);
	}
	else if(m_lexicalAnalyzer->lexeme().type == LexemeDivide) {
		result.value.setValue(RpnFunctionDivide);
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
	result.type = RpnElementFunction;

	if (m_lexicalAnalyzer->lexeme().type == LexemePlus) {
		result.value.setValue(RpnFunctionPlus);
	}
	else if(m_lexicalAnalyzer->lexeme().type == LexemeMinus) {
		result.value.setValue(RpnFunctionMinus);
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

// Constant = Identifier (formal arguments are processed here as well)
RpnElement SyntaxAnalyzer::constant()
{
	RpnElement result;
	QString constName = m_lexicalAnalyzer->lexeme().value;
	
	if (m_workingParams.contains(constName)) {
		// it is an argument
		result.type = RpnElementParam;
		result.value = m_workingParams.indexOf(constName);
	}
	
	else if (m_consts.contains(constName)) {
		result.type = RpnElementOperand;
		result.value = m_consts.value(constName);		
	}
	
	else {
		throw Exception(tr("Undeclared identifier ‘%1’").arg(constName));
	}
	
	m_lexicalAnalyzer->nextLexeme();	
	return result;
}

// FormalArgument = Identifier
void SyntaxAnalyzer::extractFormalArgument()
{
	if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
		throw Exception(tr("Identifier expected if argument list"));
	}
	
	QString argumentName = m_lexicalAnalyzer->lexeme().value;
	if (m_workingParams.contains(argumentName)) {
		throw Exception(tr("Argument ‘%1’ is already in the list").arg(argumentName));
	}
	m_workingParams.append(argumentName);
	
	m_lexicalAnalyzer->nextLexeme();
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


