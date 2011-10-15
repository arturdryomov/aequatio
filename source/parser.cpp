#include "parser.h"
#include "lexer.h"
#include "prettyprinter.h"
#include "builtin/constant.h"

Parser::Parser(QObject *parent) :
	QObject(parent),
	m_document(0),
	m_codeGenerator(0),
	m_lexer(new Lexer(this)),
	m_calculator(0)
{
}

Parser::~Parser()
{
	delete m_lexer;
}

QString Parser::process(const QString &input, Document *document)
{
	if (document == 0) {
		THROW(EInternal());
	}
	m_document = document;

	m_codeGenerator = new CodeGenerator(m_document);
	m_calculator = new Calculator(m_document, this);

	// Perform lexical analyzis
	m_lexer->parse(input);
	if (m_lexer->lexeme().type == LexemeEol) {
		THROW(EEmptyInput());
	}

	// Process the command
	QString result = command();

	delete m_calculator;
	m_calculator = 0;
	delete m_codeGenerator;
	m_codeGenerator = 0;

	return result;
}

// Command = ConstDeclaration | Expression | FuncDeclaration
QString Parser::command()
{
	QString result;

	// const declaration
	if (m_lexer->lexeme().type == LexemeConst) {
		result = constDeclaration();
		ensureNoMoreLexemes();
	}

	// func declaration
	else if (m_lexer->lexeme().type == LexemeFunc) {
		result = functionDeclaration();
		ensureNoMoreLexemes();
	}

	// Expression
	else {
		Rpn::CodeThread codeThread = expression(); // convert expression to RPN
		ensureNoMoreLexemes();
		Rpn::Operand expressionResult = m_calculator->calculate(codeThread);

		PrettyPrinter printer(m_document);
		result = QString("%1 = %2").arg(printer.process(codeThread)).arg(expressionResult.toString());
	}

	return result;
}

// ConstDeclaration = 'const' Identifier '=' Expression
QString Parser::constDeclaration()
{
	// 'const'
	if (m_lexer->lexeme().type != LexemeConst) {
		THROW(EInternal());
	}
	m_lexer->nextLexeme();

	// Identifier
	if (m_lexer->lexeme().type != LexemeIdentifier) {
		THROW(EExpected(tr("Identifier after “const”")));
	}
	QString constName = m_lexer->lexeme().value;
	m_lexer->nextLexeme();

	// '='
	if (m_lexer->lexeme().type != LexemeEqual) {
		THROW(EExpected(tr("Sign “=” after identifier")));
	}
	m_lexer->nextLexeme();

	// Expression
	Rpn::CodeThread constThread = expression();
	m_lexer->nextLexeme();
	Rpn::Operand value = m_calculator->calculate(constThread);
	m_codeGenerator->addConstant(constName, value);

	return tr("Constant declared: %1").arg(m_document->prettyPrintedConstant(constName));
}

// FunctionDeclaration = 'func' Indenifier '(' FormalArgument
//		{ ',' FormalArgument} ')' '=' Expression
QString Parser::functionDeclaration()
{
	/* Get function name */

	// 'func'
	if (m_lexer->lexeme().type != LexemeFunc) {
		THROW(EInternal());
	}
	m_lexer->nextLexeme();

	// identifier
	if (m_lexer->lexeme().type != LexemeIdentifier) {
		THROW(EExpected(tr("Identifier after “func”")));
	}
	QString functionName = m_lexer->lexeme().value;
	m_lexer->nextLexeme();


	/* Get formal arguments and save them to list */

	if (m_lexer->lexeme().type != LexemeOpeningBracket) {
		THROW(EExpected(tr("Opening bracket after function name")));
	}

	m_workingArguments.clear();
	do {
		m_lexer->nextLexeme();
		m_workingArguments.append(formalArgument());
	} while (m_lexer->lexeme().type == LexemeComma);

	if (m_lexer->lexeme().type != LexemeClosingBracket) {
		THROW(EExpected(tr("Closing bracket after formal arguments list")));
	}
	m_lexer->nextLexeme();

	if (m_lexer->lexeme().type != LexemeEqual) {
		THROW(EExpected(tr("Sign “=” after closing bracket")));
	}
	m_lexer->nextLexeme();


	/* Parse the function body and save it */

	Rpn::CodeThread functionBody = expression();
	// This adds function to m_document
	m_codeGenerator->addFunction(functionName, m_workingArguments, functionBody);

	return tr("Function declared: %1").arg(m_document->prettyPrintedFunction(functionName));
}

// Expression = (Summand {SummOperator Summand}) | Vector
Rpn::CodeThread Parser::expression()
{
	/* Vector */
	if (m_lexer->lexeme().type == LexemeOpeningSquareBracket) {
		return vector();
	}

	/* (Summand {SummOperator Summand}) */

	// First obligatory summand
	Rpn::CodeThread result = summand();

	// {SummOperator Summand} section
	while (CheckLexeme::isSummOperation(m_lexer->lexeme())) {
		BinaryOperation operation = summOperation();
		Rpn::CodeThread rightOperand = summand();
		result = m_codeGenerator->generateBinaryOperation(operation, result, rightOperand);
	}

	return result;
}

// Vector = '[' Expression, { ',' Expression } ']'
Rpn::CodeThread Parser::vector()
{
	return m_codeGenerator->packVector(extractVector());
}

// Extracts vector recursively
Rpn::Vector Parser::extractVector()
{
	if (m_lexer->lexeme().type != LexemeOpeningSquareBracket) {
		THROW(EExpected(tr("Opening bracket for vector initialization")));
	}

	m_lexer->nextLexeme();

	Rpn::Vector result;

	// Multi-dimensional vector. Recursive calls
	if (m_lexer->lexeme().type == LexemeOpeningSquareBracket) {
		m_lexer->previousLexeme();
		QList<Rpn::Vector> elements;
		do {
			m_lexer->nextLexeme();
			elements << extractVector();
		} while (m_lexer->lexeme().type == LexemeComma);
		result = m_codeGenerator->generateVector(elements);
	}

	// One-dimensional vector
	else {
		m_lexer->previousLexeme();
		QList<Number> elements;
		do {
			m_lexer->nextLexeme();
			Rpn::CodeThread elementThread = expression();
			Rpn::Operand operand = m_calculator->calculate(elementThread);
			if (operand.type != Rpn::OperandNumber) {
				THROW(EIncorrectVectorInitialization());
			}
			elements << operand.value.value<Number>();
		} while (m_lexer->lexeme().type == LexemeComma);
		result = m_codeGenerator->generateVector(elements);
	}

	if (m_lexer->lexeme().type != LexemeClosingSquareBracket) {
		THROW(EExpected(tr("Closing bracket for vector initialization")));
	}
	m_lexer->nextLexeme();

	return result;
}

// Function = Name'(' ActualArgument{ ',' ActualArgument}')'
Rpn::CodeThread Parser::function()
{
	// Function name
	if (m_lexer->lexeme().type != LexemeIdentifier) {
		THROW(EInternal());
	}
	QString functionName = m_lexer->lexeme().value;

	// opening bracket
	m_lexer->nextLexeme();
	if (m_lexer->lexeme().type != LexemeOpeningBracket) {
		THROW(EExpected(tr("Opening bracket after function name")));
	}


	// Parse actual arguments and add them to list of threads
	QList<Rpn::CodeThread> actualArguments;
	do {
		m_lexer->nextLexeme();
		actualArguments << expression();
	} while (m_lexer->lexeme().type == LexemeComma);

	// Closing bracket
	if (m_lexer->lexeme().type != LexemeClosingBracket) {
		THROW(EExpected(tr("Closing bracket after arguments list")));
	}
	m_lexer->nextLexeme();

	// Result
	return m_codeGenerator->generateFunction(functionName, actualArguments);
}

// Factor = (UnaryOp Factor) | (PowerBase ['^' Factor])
Rpn::CodeThread Parser::factor()
{
	Rpn::CodeThread result;

	// UnaryOp Factor
	if (CheckLexeme::isUnaryOperation(m_lexer->lexeme())) {
		LexemeType lexemeType = m_lexer->lexeme().type;
		m_lexer->nextLexeme();
		if (lexemeType == LexemeMinus) {
			result = m_codeGenerator->generateUnaryMinus(factor());
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
		result = powerBase();

		// ['^' Factor]
		if (m_lexer->lexeme().type == LexemePower) {
			m_lexer->nextLexeme();
			result = m_codeGenerator->generateBinaryOperation(BinaryOperationPower, result, factor());
		}
	}

	return result;
}

// PowerBase = Number | Identifier | '('Expression')'
Rpn::CodeThread Parser::powerBase()
{
	Rpn::CodeThread result;

	// Number
	if (m_lexer->lexeme().type == LexemeNumber) {
		result = m_codeGenerator->generateNumber(number());
	}

	// Constant | Function
	else if (m_lexer->lexeme().type == LexemeIdentifier) {
		result = identifier();
	}

	// '('Expression')'
	else if (m_lexer->lexeme().type == LexemeOpeningBracket) {
		m_lexer->nextLexeme();
		result = expression();

		if (m_lexer->lexeme().type != LexemeClosingBracket) {
			THROW(EExpected(tr("Closing bracket")));
		}

		m_lexer->nextLexeme();
	}

	else {
		THROW(EIncorrectInput()); // NOTE: Exception type is possibly to be more specific.
	}

	return result;
}

// MultOperation = '*' | '/'
BinaryOperation Parser::multOperation()
{
	BinaryOperation result;

	if (m_lexer->lexeme().type == LexemeMultiply) {
		result = BinaryOperationMultiply;
	}
	else if(m_lexer->lexeme().type == LexemeDivide) {
		result = BinaryOperationDivide;
	}
	else {
		THROW(EInternal());
	}

	m_lexer->nextLexeme();
	return result;
}

// Summand = Factor {MultOperator Factor}
Rpn::CodeThread Parser::summand()
{
	// First obligatory factor
	Rpn::CodeThread result = factor();

	// {MultOperation Factor} section
	while (CheckLexeme::isMultOperation(m_lexer->lexeme())) {
		BinaryOperation operation = multOperation();
		Rpn::CodeThread rightOperand = factor();
		result = m_codeGenerator->generateBinaryOperation(operation, result, rightOperand);
	}

	return result;
}

// Identifier = Constant | Formal argument | Function name | Function
Rpn::CodeThread Parser::identifier()
{
	QString name = m_lexer->lexeme().value;
	m_lexer->nextLexeme();
	if (m_lexer->lexeme().type != LexemeOpeningBracket) {

		// Constant | Formal argument | Function name

		// Formal argument
		if (m_workingArguments.contains(name)) {
			return m_codeGenerator->generateFormalArgument(name);
		}

		// Constant
		if (m_document->containsConstant(name) || BuiltIn::Constant::constants().contains(name)) {
			return m_codeGenerator->generateConstant(name);
		}

		// Function name
		if (m_document->containsFunction(name) || BuiltIn::Function::functions().contains(name)) {
			return m_codeGenerator->generateFunctonNameOperand(name);
		}

		THROW(EUndeclaredUsed(name, EUndeclaredUsed::Constant));

	}

	// Function
	else {
		m_lexer->previousLexeme();
		return function();
	}

}

// SummOperation = '+' | '-'
BinaryOperation Parser::summOperation()
{
	BinaryOperation result;

	if (m_lexer->lexeme().type == LexemePlus) {
		result = BinaryOperationPlus;
	}
	else if(m_lexer->lexeme().type == LexemeMinus) {
		result = BinaryOperationMinus;
	}
	else {
		THROW(EInternal());
	}

	m_lexer->nextLexeme();
	return result;
}

Number Parser::number()
{
	Number result = stringToNumber(m_lexer->lexeme().value);
	m_lexer->nextLexeme();
	return result;
}

// FormalArgument = Identifier //NOTE: this is formal argument description, not using
QString Parser::formalArgument()
{
	if (m_lexer->lexeme().type != LexemeIdentifier) {
		THROW(EExpected(tr("Argument name")));
	}

	QString argumentName = m_lexer->lexeme().value;
	if (m_workingArguments.contains(argumentName)) {
		THROW(EFormalArgumentReused(argumentName));
	}
	m_lexer->nextLexeme();

	return argumentName;
}

void Parser::ensureNoMoreLexemes()
{
	if (m_lexer->lexeme().type != LexemeEol) {
		THROW(EIncorrectInput());
	}
}

QList<Rpn::Argument> Parser::functionArguments(const QString &functionName)
{
	if (BuiltIn::Function::functions().contains(functionName)) {
		return BuiltIn::Function::functions().value(functionName)->requiredArguments();
	}

	else if (m_document->containsFunction(functionName)) {
		return m_document->function(functionName).arguments;
	}
	else {
		THROW(EIncorrectRpnCode());
	}
}


// CheckLexeme class methods

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


// EFormalArgumentReused class methods

EFormalArgumentReused::EFormalArgumentReused(const QString &argumentName) :
	m_argumentName(argumentName)
{
}

QString EFormalArgumentReused::message()
{
	return tr("There is already argument named “%1” in the arguments list.")
		.arg(m_argumentName);
}


// EIncorrectVectorInitialization class methods

QString EIncorrectVectorInitialization::message()
{
	return tr("Incorrect vector initialization. It should look like "
		"<i>[number1, number2, ..., numberN </i>");
}
