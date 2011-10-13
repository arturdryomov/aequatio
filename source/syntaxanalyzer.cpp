#include "syntaxanalyzer.h"
#include "lexicalanalyzer.h"
#include "parsingexceptions.h"
#include "prettyprinter.h"
#include "builtin/constant.h"

SyntaxAnalyzer::SyntaxAnalyzer(QObject *parent) :
	QObject(parent),
	m_document(0),
	m_codeGenerator(0),
	m_lexicalAnalyzer(new LexicalAnalyzer(this)),
	m_exprCalculator(0)
{
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
}

QString SyntaxAnalyzer::process(const QString &input, Document *document)
{
	m_document = document;
	m_codeGenerator = new CodeGenerator(document);
	m_exprCalculator = new ExprCalculator(this);
	m_exprCalculator->setDocument(document);

	// perform lexical analyzis
	m_lexicalAnalyzer->parse(input);
	if (m_lexicalAnalyzer->lexeme().type == LexemeEol) {
		THROW(EEmptyInput());
	}

	// Process the command
	QString result = command();

	delete m_exprCalculator;
	m_exprCalculator = 0;
	delete m_codeGenerator;
	m_codeGenerator = 0;

	return result;
}

// Command = ConstDeclaration | Expression | FuncDeclaration
QString SyntaxAnalyzer::command()
{
	QString result;

	// const declaration
	if (m_lexicalAnalyzer->lexeme().type == LexemeConst) {
		result = constDeclaration();
		ensureNoMoreLexemes();
	}

	// func declaration
	else if (m_lexicalAnalyzer->lexeme().type == LexemeFunc) {
		result = functionDeclaration();
		ensureNoMoreLexemes();
	}

	// expression
	else {
		Rpn::CodeThread codeThread = expression(); // convert expression to RPN
		ensureNoMoreLexemes();
		Rpn::Operand expressionResult = m_exprCalculator->calculate(codeThread);

		PrettyPrinter printer;
		printer.setDocument(m_document);
		result = QString("%1 = %2").arg(printer.process(codeThread)).arg(expressionResult.toString());
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
	Rpn::CodeThread constThread = expression();
	m_lexicalAnalyzer->nextLexeme();
	Rpn::Operand value = m_exprCalculator->calculate(constThread);
	m_codeGenerator->addConstant(constName, value);

	return tr("Constant declared: %1").arg(m_document->prettyPrintedConstant(constName));
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

	Rpn::CodeThread functionBody = expression();
	// this adds function to m_document
	m_codeGenerator->addFunction(functionName, m_workingArguments, functionBody);

	return tr("Function declared: %1").arg(m_document->prettyPrintedFunction(functionName));
}

// Expression = (Summand {SummOperator Summand}) | Vector
Rpn::CodeThread SyntaxAnalyzer::expression()
{
	/* Vector */
	if (m_lexicalAnalyzer->lexeme().type == LexemeOpeningSquareBracket) {
		m_lexicalAnalyzer->previousLexeme();
		return vector();
	}

	/* (Summand {SummOperator Summand}) */

	// first obligatory summand
	Rpn::CodeThread result = summand();

	// {SummOperator Summand} section
	while (CheckLexeme::isSummOperation(m_lexicalAnalyzer->lexeme())) {
		BinaryOperation operation = summOperation();
		Rpn::CodeThread rightOperand = summand();
		result = m_codeGenerator->generateBinaryOperation(operation, result, rightOperand);
	}

	return result;
}

// Vector = '[' Expression, { ',' Expression } ']'
Rpn::CodeThread SyntaxAnalyzer::vector()
{
	return m_codeGenerator->packVector(extractVector());
}

// extracts vector recursively
Rpn::Vector SyntaxAnalyzer::extractVector()
{
	if (m_lexicalAnalyzer->lexeme().type != LexemeOpeningSquareBracket) {
		THROW(ELexemeExpected(tr("Opening bracket for vector initialization")));
	}

	m_lexicalAnalyzer->nextLexeme();

	Rpn::Vector result;

	// Multi-dimensional vector. Recursive calls
	if (m_lexicalAnalyzer->lexeme().type == LexemeOpeningSquareBracket) {
		m_lexicalAnalyzer->previousLexeme();
		QList<Rpn::Vector> elements;
		do {
			m_lexicalAnalyzer->nextLexeme();
			elements << extractVector();
		} while (m_lexicalAnalyzer->lexeme().type == LexemeComma);
		result = m_codeGenerator->generateVector(elements);
	}	

	// One-dimensional vector
	else {
		m_lexicalAnalyzer->previousLexeme();
		QList<Number> elements;
		do {
			m_lexicalAnalyzer->nextLexeme();
			Rpn::CodeThread elementThread = expression();
			Rpn::Operand operand = m_exprCalculator->calculate(elementThread);
			if (operand.type != Rpn::OperandNumber) {
				THROW(EIncorrectVectorInitialization());
			}
			elements << operand.value.value<Number>();
		} while (m_lexicalAnalyzer->lexeme().type == LexemeComma);
		result = m_codeGenerator->generateVector(elements);
	}

	if (m_lexicalAnalyzer->lexeme().type != LexemeClosingSquareBracket) {
		THROW(ELexemeExpected(tr("Closing bracket for vector initialization")));
	}
	m_lexicalAnalyzer->nextLexeme();

	return result;
}

// Function = Name'(' ActualArgument{ ',' ActualArgument}')'
Rpn::CodeThread SyntaxAnalyzer::function()
{
	// function name
	if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
		THROW(EInternal());
	}	
	QString functionName = m_lexicalAnalyzer->lexeme().value;	

	// opening bracket
	m_lexicalAnalyzer->nextLexeme();	
	if (m_lexicalAnalyzer->lexeme().type != LexemeOpeningBracket) {
		THROW(ELexemeExpected(tr("Opening bracket after function name")));
	}


	// Parse actual arguments and add them to list of threads
	QList<Rpn::CodeThread> actualArguments;
	do {
		m_lexicalAnalyzer->nextLexeme();
		actualArguments << expression();
	} while (m_lexicalAnalyzer->lexeme().type == LexemeComma);

	// closing bracket
	if (m_lexicalAnalyzer->lexeme().type != LexemeClosingBracket) {
		THROW(ELexemeExpected(tr("Closing bracket after arguments list")));
	}	
	m_lexicalAnalyzer->nextLexeme();

	// result
	return m_codeGenerator->generateFunction(functionName, actualArguments);
}

// Factor = (UnaryOp Factor) | (PowerBase ['^' Factor])
Rpn::CodeThread SyntaxAnalyzer::factor()
{
	Rpn::CodeThread result;

	// UnaryOp Factor
	if (CheckLexeme::isUnaryOperation(m_lexicalAnalyzer->lexeme())) {
		LexemeType lexemeType = m_lexicalAnalyzer->lexeme().type;
		m_lexicalAnalyzer->nextLexeme();
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
		if (m_lexicalAnalyzer->lexeme().type == LexemePower) {
			m_lexicalAnalyzer->nextLexeme();
			result = m_codeGenerator->generateBinaryOperation(BinaryOperationPower, result, factor());
		}
	}

	return result;
}

// PowerBase = Number | Identifier | '('Expression')'
Rpn::CodeThread SyntaxAnalyzer::powerBase()
{
	Rpn::CodeThread result;

	// Number
	if (m_lexicalAnalyzer->lexeme().type == LexemeNumber) {
		result = m_codeGenerator->generateNumber(number());
	}

	// Constant | Function
	else if (m_lexicalAnalyzer->lexeme().type == LexemeIdentifier) {
		result = identifier();
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
BinaryOperation SyntaxAnalyzer::multOperation()
{
	BinaryOperation result;

	if (m_lexicalAnalyzer->lexeme().type == LexemeMultiply) {
		result = BinaryOperationMultiply;
	}
	else if(m_lexicalAnalyzer->lexeme().type == LexemeDivide) {
		result = BinaryOperationDivide;
	}
	else {
		THROW(EInternal());
	}

	m_lexicalAnalyzer->nextLexeme();
	return result;
}

// Summand = Factor {MultOperator Factor}
Rpn::CodeThread SyntaxAnalyzer::summand()
{
	// first obligatory factor
	Rpn::CodeThread result = factor();

	// {MultOperation Factor} section
	while (CheckLexeme::isMultOperation(m_lexicalAnalyzer->lexeme())) {
		BinaryOperation operation = multOperation();
		Rpn::CodeThread rightOperand = factor();
		result = m_codeGenerator->generateBinaryOperation(operation, result, rightOperand);
	}

	return result;
}

// Identifier = Constant | Formal argument | Function name | Function
Rpn::CodeThread SyntaxAnalyzer::identifier()
{
	QString name = m_lexicalAnalyzer->lexeme().value;
	m_lexicalAnalyzer->nextLexeme();
	if (m_lexicalAnalyzer->lexeme().type != LexemeOpeningBracket) {

		// Constant | Formal argument | Function name

		// formal argument
		if (m_workingArguments.contains(name)) {
			return m_codeGenerator->generateFormalArgument(name);
		}

		// constant
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
		m_lexicalAnalyzer->previousLexeme();
		return function();
	}

}

// SummOperation = '+' | '-'
BinaryOperation SyntaxAnalyzer::summOperation()
{
	BinaryOperation result;

	if (m_lexicalAnalyzer->lexeme().type == LexemePlus) {
		result = BinaryOperationPlus;
	}
	else if(m_lexicalAnalyzer->lexeme().type == LexemeMinus) {
		result = BinaryOperationMinus;
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

// FormalArgument = Identifier //NOTE: this is formal argument description, not using
QString SyntaxAnalyzer::formalArgument()
{
	if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
		THROW(ELexemeExpected(tr("Argument name")));
	}
	
	QString argumentName = m_lexicalAnalyzer->lexeme().value;
	if (m_workingArguments.contains(argumentName)) {
		THROW(EFormalArgumentReused(argumentName));
	}
	m_lexicalAnalyzer->nextLexeme();

	return argumentName;
}

void SyntaxAnalyzer::ensureNoMoreLexemes()
{
	if (m_lexicalAnalyzer->lexeme().type != LexemeEol) {
		THROW(EParsing());
	}
}

QList<Rpn::Argument> SyntaxAnalyzer::functionArguments(const QString &functionName)
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
