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

ProcessingResult SyntaxAnalyzer::process(const QString &input)
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
ProcessingResult SyntaxAnalyzer::command()
{
	ProcessingResult result;

	// const declaration
	if (m_lexicalAnalyzer->lexeme().type == LexemeConst) {
		ConstantDescription constant = constDeclaration();
		ensureNoMoreLexemes();
		result.type = ResultConstantDeclared;
		result.data.setValue(constant);
	}

	// func declaration
	else if (m_lexicalAnalyzer->lexeme().type == LexemeFunc) {
		FunctionDescription function = functionDeclaration();
		ensureNoMoreLexemes();
		result.type = ResultFunctionDeclared;
		result.data.setValue(function);
	}

	// expression
	else {
		RpnCodeThread codeThread = expression(); // convert expression to RPN
		ensureNoMoreLexemes();
		ExpressionDescription expression = m_exprCalculator->calculate(codeThread);
		result.type = ResultExpressionCalculated;
		result.data.setValue(expression);
	}

	return result;
}

// ConstDeclaration = 'const' Identifier '=' Expression
ConstantDescription SyntaxAnalyzer::constDeclaration()
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
	ExpressionDescription expression = m_exprCalculator->calculate(constThread);
	// constant values can only be of Number type now
	if (expression.result.type != RpnOperandNumber) {
		THROW(EIncorrectConstantDeclaration());
	}
	m_lexicalAnalyzer->nextLexeme();

	// add constant to list
	return m_exprCalculator->addConstant(constName, expression.result.value.value<Number>());
}

// FunctionDeclaration = 'func' Indenifier '(' FormalArgument
//		{ ',' FormalArgument} ')' '=' Expression
FunctionDescription SyntaxAnalyzer::functionDeclaration()
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

	m_workingArguments.clear();
	do {
		m_lexicalAnalyzer->nextLexeme();
		m_workingArguments.append(formalArgument());
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

	return m_exprCalculator->addFunction(functionName, function);
}

// Expression = Summand {SummOperator Summand}
RpnCodeThread SyntaxAnalyzer::expression()
{
	RpnCodeThread result;

	// first obligatory summand
	result << summand();

	// {SummOperator Summand} section
	while (CheckLexeme::isSummOperation(m_lexicalAnalyzer->lexeme())) {
		RpnElement operation = summOperation();
		result << summand() << operation;
	}

	return result;
}

// Vector = '[' Expression, { ',' Expression } ']'
RpnCodeThread SyntaxAnalyzer::vector()
{
	RpnCodeThread result;
	QList<Number> vectorElements;

	if (m_lexicalAnalyzer->lexeme().type != LexemeOpeningSquareBracket) {
		THROW(ELexemeExpected(tr("Opening bracket for vector initialization")));
	}

	do {
		m_lexicalAnalyzer->nextLexeme();
		if (m_lexicalAnalyzer->lexeme().type != LexemeNumber) {
			THROW(ELexemeExpected(tr("Number")));
		}
		vectorElements << stringToNumber(m_lexicalAnalyzer->lexeme().value);
		m_lexicalAnalyzer->nextLexeme();
	} while (m_lexicalAnalyzer->lexeme().type == LexemeComma);

	if (m_lexicalAnalyzer->lexeme().type != LexemeClosingSquareBracket) {
		THROW(ELexemeExpected(tr("Closing bracket for vector initialization")));
	}
	m_lexicalAnalyzer->nextLexeme();

	RpnOperand operand(RpnOperandVector, QVariant::fromValue(vectorElements) );
	result << RpnElement(RpnElementOperand, QVariant::fromValue(operand));
	return result;
}

// Function = Identifier'(' ActualArgument{ ',' ActualArgument}')'
RpnCodeThread SyntaxAnalyzer::function()
{
	RpnCodeThread result;
	
	// Get function name and ensure it exists

	if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
		THROW(EInternal());
	}	
	QString functionName = m_lexicalAnalyzer->lexeme().value;

	if (!m_exprCalculator->isFunction(functionName)) {
		THROW(EUndeclaredUsed(functionName, EUndeclaredUsed::Function));
	}
	QList<RpnArgument> formalArguments = m_exprCalculator->functionArguments(functionName);

	m_lexicalAnalyzer->nextLexeme();	

	if (m_lexicalAnalyzer->lexeme().type != LexemeOpeningBracket) {
		THROW(ELexemeExpected(tr("Opening bracket after function name")));
	}
	
	// Parse actual arguments and add them to thread.
	// Ensure their count equals formal arguments count and check for argument types compability.

	int actualArgumentIndex = 0;
	do {
		m_lexicalAnalyzer->nextLexeme();
		if (actualArgumentIndex >= formalArguments.count()) {
			THROW(EWrongArgumentsCount(functionName, formalArguments.count()));
		}
		result << actualArgument(formalArguments.at(actualArgumentIndex));
		++actualArgumentIndex;
	} while (m_lexicalAnalyzer->lexeme().type == LexemeComma);
	
	if (actualArgumentIndex != formalArguments.count()) {
		THROW(EWrongArgumentsCount(functionName, formalArguments.count()));
	}

	if (m_lexicalAnalyzer->lexeme().type != LexemeClosingBracket) {
		THROW(ELexemeExpected(tr("Closing bracket after arguments list")));
	}	
	m_lexicalAnalyzer->nextLexeme();
	
	// Add to thread a function call element
	result << RpnElement(RpnElementFunctionCall, functionName);
	
	return result;
}

// Factor = (UnaryOp Factor) | (PowerBase ['^' Factor])
RpnCodeThread SyntaxAnalyzer::factor()
{
	RpnCodeThread result;

	// UnaryOp Factor
	if (CheckLexeme::isUnaryOperation(m_lexicalAnalyzer->lexeme())) {
		LexemeType lexemeType = m_lexicalAnalyzer->lexeme().type;
		m_lexicalAnalyzer->nextLexeme();
		if (lexemeType == LexemeMinus) {
			RpnElement unaryMinus(RpnElementFunctionCall, QVariant::fromValue(RpnFunctionUnaryMinus));
			result << factor() << unaryMinus;
		}

		else if (lexemeType == LexemePlus) {
			result = factor();
		}

		else {
			THROW(EInternal());
		}
	}

	// PowerBase ['^' Factor]
	else {
		result << powerBase();

		// ['^' Factor]
		if (m_lexicalAnalyzer->lexeme().type == LexemePower) {
			RpnElement power(RpnElementFunctionCall, QVariant::fromValue(RpnFunctionPower));
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
		RpnOperand operand(RpnOperandNumber, number());
		result << RpnElement(RpnElementOperand, QVariant::fromValue(operand));
	}

	// Constant | Function
	else if (m_lexicalAnalyzer->lexeme().type == LexemeIdentifier) {
		// Constant
		m_lexicalAnalyzer->nextLexeme();
		if (m_lexicalAnalyzer->lexeme().type != LexemeOpeningBracket) {
			m_lexicalAnalyzer->previousLexeme();
			result << constant();
		}
		// Function
		else {
			m_lexicalAnalyzer->previousLexeme();
			result << function();
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
	RpnElement result(RpnElementFunctionCall);

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
	result << factor();

	// {MultOperation Factor} section
	while (CheckLexeme::isMultOperation(m_lexicalAnalyzer->lexeme())) {
		RpnElement operation = multOperation();
		result << factor() << operation;
	}

	return result;
}

// SummOperation = '+' | '-'
RpnElement SyntaxAnalyzer::summOperation()
{
	RpnElement result(RpnElementFunctionCall);

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
	Number result = stringToNumber(m_lexicalAnalyzer->lexeme().value);
	m_lexicalAnalyzer->nextLexeme();
	return result;
}

// Constant = Identifier (formal arguments are processed here as well)
RpnElement SyntaxAnalyzer::constant()
{
	QString constName = m_lexicalAnalyzer->lexeme().value;
	m_lexicalAnalyzer->nextLexeme();

	// it is a formal argument
	RpnArgument possibleArgument(RpnOperandNumber, constName, QVariant());
	if (m_workingArguments.contains(possibleArgument)) {
		return RpnElement(RpnElementArgument, constName);
	}
	
	// it is a constant
	else if (m_exprCalculator->isConstant(constName)) {
		return RpnElement(RpnElementConstant, constName);
	}
	
	else {
		THROW(EUndeclaredUsed(constName, EUndeclaredUsed::Constant));
	}
}

// FormalArgument = Identifier
RpnArgument SyntaxAnalyzer::formalArgument()
{
	if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
		THROW(ELexemeExpected(tr("Argument name")));
	}
	
	QString argumentName = m_lexicalAnalyzer->lexeme().value;
	// user-defined functions are now can take only Numbers as arguments
	RpnArgument argument(RpnOperandNumber, argumentName);
	if (m_workingArguments.contains(argument)) {
		THROW(EFormalArgumentReused(argumentName));
	}
	m_lexicalAnalyzer->nextLexeme();

	return argument;
}

RpnCodeThread SyntaxAnalyzer::actualArgument(const RpnArgument &correspondingFormalArgument)
{
	switch (correspondingFormalArgument.type) {

		case RpnOperandNumber:
			return expression();

		case RpnOperandVector:
			return vector();

		case RpnOperandFunctionName: {
			if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
				THROW(ELexemeExpected(tr("Function name")));
			}
			QString argumentFunctionName = m_lexicalAnalyzer->lexeme().value;

			// check argument count if number of arguments is fixed
			if (m_exprCalculator->functionArguments(argumentFunctionName).first().info != QVariant()) {
				if (m_exprCalculator->functionArguments(argumentFunctionName).count()
					!= correspondingFormalArgument.info.value<int>()) {
					THROW(EIncorrectFunctionArgument(argumentFunctionName));
				}
			}

			RpnOperand rpnOperand(RpnOperandFunctionName, argumentFunctionName);
			m_lexicalAnalyzer->nextLexeme();
			return RpnCodeThread() << RpnElement(RpnElementOperand, QVariant::fromValue(rpnOperand));
		}

		default:
			THROW(EInternal());
	}
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
