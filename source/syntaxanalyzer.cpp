#include "syntaxanalyzer.h"
#include "lexicalanalyzer.h"
#include "parsingexceptions.h"

#include <QMetaType>

SyntaxAnalyzer::SyntaxAnalyzer(QObject *parent) :
	QObject(parent),
	m_lexicalAnalyzer(new LexicalAnalyzer(this)),
	m_exprCalculator(new ExprCalculator(this))
{
	connect(m_exprCalculator, SIGNAL(constantsListChanged()), SIGNAL(constantsListChanged()));
	connect(m_exprCalculator, SIGNAL(functionsListChanged()), SIGNAL(functionsListChanged()));
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
}

QString SyntaxAnalyzer::process(const QString &input)
{
	// perform lexical analyzis
	m_lexicalAnalyzer->parse(input);
	if (m_lexicalAnalyzer->lexeme().type == LexemeEol) {
		THROW(EEmptyInput());
	}

	// Process the command: const declaration, const function or expression
	return command();
}

QList<ConstantDescription> SyntaxAnalyzer::constantsList()
{
	return m_exprCalculator->constantsList();
}

QList<FunctionDescription> SyntaxAnalyzer::functionsList()
{
	return m_exprCalculator->functionsList();
}

// Command = ConstDeclaration | Expression | FuncDeclaration
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
		RpnCodeThread codeThread = expression(); // convert expression to RPN
		ensureNoMoreLexemes();
		result = QString::number(m_exprCalculator->calculate(codeThread), 'f', 15); // calculate
	}

	return result;
}

// ConstDeclaration = 'const' Identifier '=' Expression
QString SyntaxAnalyzer::constDeclaration()
{
	// 'const'
	if (m_lexicalAnalyzer->lexeme().type != LexemeConst) {
		THROW(EInternal());
	}
	m_lexicalAnalyzer->nextLexeme();

	// Identifier
	if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
		THROW(ELexemeExpected(tr("Identifier after “const”")));
	}
	QString constName = m_lexicalAnalyzer->lexeme().value;
	m_lexicalAnalyzer->nextLexeme();

	// '='
	if (m_lexicalAnalyzer->lexeme().type != LexemeEqual) {
		THROW(ELexemeExpected(tr("Sign “=” after identifier")));
	}
	m_lexicalAnalyzer->nextLexeme();

	// Expression
	RpnCodeThread constThread = expression();
	Number constValue = m_exprCalculator->calculate(constThread);
	m_lexicalAnalyzer->nextLexeme();

	// add constant to list
	m_exprCalculator->addConstant(constName, constValue);	

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
		THROW(EInternal());
	}
	m_lexicalAnalyzer->nextLexeme();

	// identifier
	if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
		THROW(ELexemeExpected(tr("Identifier after “func”")));
	}
	QString functionName = m_lexicalAnalyzer->lexeme().value;
	m_lexicalAnalyzer->nextLexeme();
	

	/* Get formal arguments and save them to list */
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeOpeningBracket) {
		THROW(ELexemeExpected(tr("Opening bracket after function name")));
	}
	
	do {
		m_lexicalAnalyzer->nextLexeme();
		extractFormalArgument();
	} while (m_lexicalAnalyzer->lexeme().type == LexemeComma);
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeClosingBracket) {
		THROW(ELexemeExpected(tr("Closing bracket after formal arguments list")));
	}
	m_lexicalAnalyzer->nextLexeme();
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeEqual) {
		THROW(ELexemeExpected(tr("Sign “=” after closing bracket")));
	}
	m_lexicalAnalyzer->nextLexeme();

	
	/* Parse the function body and save it */
	
	RpnFunction function;
	function.arguments = m_workingArguments;
	function.codeThread = expression();
	m_workingArguments.clear();
	m_exprCalculator->addFunction(functionName, function);
	
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
		THROW(EInternal());
	}	
	QString functionName = m_lexicalAnalyzer->lexeme().value;		
	int formalArgumentsCount;
	if (m_exprCalculator->isFunction(functionName)) {
		formalArgumentsCount = m_exprCalculator->functionArgumentsCount(functionName);
	} 	
	else {
		THROW(EUndeclaredUsed(functionName, EUndeclaredUsed::Function));
	}	
	m_lexicalAnalyzer->nextLexeme();	
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeOpeningBracket) {
		THROW(ELexemeExpected(tr("Opening bracket after function name")));
	}
	
	// Parse actual arguments and add them to tread. 
	// Ensure their count equals formal arguments count
	int actualArgumentsCount = 0;
	do {
		m_lexicalAnalyzer->nextLexeme();
		result << expression();
		actualArgumentsCount++;
		if (actualArgumentsCount > formalArgumentsCount) {
			THROW(EWrongArgumentsCount(functionName, formalArgumentsCount, actualArgumentsCount));
		}		
	} while (m_lexicalAnalyzer->lexeme().type == LexemeComma);
	
	if (actualArgumentsCount != formalArgumentsCount) {
		THROW(EWrongArgumentsCount(functionName, formalArgumentsCount, actualArgumentsCount));
	}
	
	if (m_lexicalAnalyzer->lexeme().type != LexemeClosingBracket) {
		THROW(ELexemeExpected(tr("Closing bracket after arguments list")));
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
			THROW(EInternal());
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
			THROW(ELexemeExpected(tr("Closing bracket")));
		}

		m_lexicalAnalyzer->nextLexeme();
	}

	else {
		THROW(EParsing()); // NOTE: Exception type is possibly to be more specific.
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
		THROW(EInternal());
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
		THROW(EInternal());
	}

	m_lexicalAnalyzer->nextLexeme();
	return result;
}

Number SyntaxAnalyzer::number()
{
	bool ok = false;
	Number result = m_lexicalAnalyzer->lexeme().value.toDouble(&ok);
	if (!ok) {
		THROW(EConversionToNumber(m_lexicalAnalyzer->lexeme().value));
	}
	m_lexicalAnalyzer->nextLexeme();
	return result;
}

// Constant = Identifier (formal arguments are processed here as well)
RpnElement SyntaxAnalyzer::constant()
{
	RpnElement result;
	QString constName = m_lexicalAnalyzer->lexeme().value;
	
	// it is a formal argument
	if (m_workingArguments.contains(constName)) {
		result.type = RpnElementArgument;
		RpnArgumentInfo argumentInfo = {m_workingArguments.indexOf(constName), constName};
		result.value = QVariant::fromValue<RpnArgumentInfo>(argumentInfo);
	}
	
	// it is a constant
	else if (m_exprCalculator->isConstant(constName)) {
		result.type = RpnElementConstant;		
		result.value = constName;		
	}
	
	else {
		THROW(EUndeclaredUsed(constName, EUndeclaredUsed::Constant));
	}
	
	m_lexicalAnalyzer->nextLexeme();	
	return result;
}

// FormalArgument = Identifier
void SyntaxAnalyzer::extractFormalArgument()
{
	if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
		THROW(ELexemeExpected(tr("Argument name")));
	}
	
	QString argumentName = m_lexicalAnalyzer->lexeme().value;
	if (m_workingArguments.contains(argumentName)) {
		THROW(EFormalArgumentReused(argumentName));
	}
	m_workingArguments.append(argumentName);
	
	m_lexicalAnalyzer->nextLexeme();
}

void SyntaxAnalyzer::ensureNoMoreLexemes()
{
	if (m_lexicalAnalyzer->lexeme().type != LexemeEol) {
		THROW(EParsing());
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
