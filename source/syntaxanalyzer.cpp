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
		Rpn::CodeThread codeThread = expression(); // convert expression to RPN
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
	Rpn::CodeThread constThread = expression();
	ExpressionDescription expression = m_exprCalculator->calculate(constThread);
	// constant values can only be of Number type now
	if (expression.result.type != Rpn::OperandNumber) {
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
	
	Rpn::Function function;
	function.arguments = m_workingArguments;
	function.codeThread = expression();

	return m_exprCalculator->addFunction(functionName, function);
}

// Expression = Summand {SummOperator Summand}
Rpn::CodeThread SyntaxAnalyzer::expression()
{
	Rpn::CodeThread result;

	// first obligatory summand
	result << summand();

	// {SummOperator Summand} section
	while (CheckLexeme::isSummOperation(m_lexicalAnalyzer->lexeme())) {
		Rpn::Element operation = summOperation();
		result << summand() << operation;
	}

	return result;
}

// Vector = '[' Expression, { ',' Expression } ']'
Rpn::CodeThread SyntaxAnalyzer::vector()
{
	Rpn::Operand operand(Rpn::OperandVector, QVariant::fromValue(extractVector()));
	Rpn::CodeThread result;
	result << Rpn::Element(Rpn::ElementOperand, QVariant::fromValue(operand));
	return result;
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
		Rpn::Vector element;
		do {
			m_lexicalAnalyzer->nextLexeme();
			element = extractVector();
			result.values << QVariant::fromValue(element.values);
		} while (m_lexicalAnalyzer->lexeme().type == LexemeComma);
		result.dimensions = element.dimensions + 1;
	}

	// One-dimensional vector
	else {
		m_lexicalAnalyzer->previousLexeme();
		do {
			m_lexicalAnalyzer->nextLexeme();
			Rpn::CodeThread elementThread = expression();
			ExpressionDescription expression = m_exprCalculator->calculate(elementThread);
			if (expression.result.type != Rpn::OperandNumber) {
				THROW(EIncorrectVectorInitialization());
			}

			result.values << expression.result.value;
		} while (m_lexicalAnalyzer->lexeme().type == LexemeComma);
		result.dimensions = 1;
	}

	if (m_lexicalAnalyzer->lexeme().type != LexemeClosingSquareBracket) {
		THROW(ELexemeExpected(tr("Closing bracket for vector initialization")));
	}
	m_lexicalAnalyzer->nextLexeme();

	return result;
}

// Function = Identifier'(' ActualArgument{ ',' ActualArgument}')'
Rpn::CodeThread SyntaxAnalyzer::function()
{
	Rpn::CodeThread result;
	
	// Get function name and ensure it exists

	if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
		THROW(EInternal());
	}	
	QString functionName = m_lexicalAnalyzer->lexeme().value;

	if (!m_exprCalculator->isFunction(functionName)) {
		THROW(EUndeclaredUsed(functionName, EUndeclaredUsed::Function));
	}
	QList<Rpn::Argument> formalArguments = m_exprCalculator->functionArguments(functionName);

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
	result << Rpn::Element(Rpn::ElementFunctionCall, functionName);
	
	return result;
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
			Rpn::Element unaryMinus(Rpn::ElementFunctionCall, QVariant::fromValue(Rpn::FunctionUnaryMinus));
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
			Rpn::Element power(Rpn::ElementFunctionCall, QVariant::fromValue(Rpn::FunctionPower));
			m_lexicalAnalyzer->nextLexeme();
			Rpn::CodeThread exponent = factor();

			result << exponent << power;
		}
	}

	return result;
}

// PowerBase = Number | Constant | Function | '('Expression')'
Rpn::CodeThread SyntaxAnalyzer::powerBase()
{
	Rpn::CodeThread result;

	// Number
	if (m_lexicalAnalyzer->lexeme().type == LexemeNumber) {
		Rpn::Operand operand(Rpn::OperandNumber, number());
		result << Rpn::Element(Rpn::ElementOperand, QVariant::fromValue(operand));
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
Rpn::Element SyntaxAnalyzer::multOperation()
{
	Rpn::Element result(Rpn::ElementFunctionCall);

	if (m_lexicalAnalyzer->lexeme().type == LexemeMultiply) {
		result.value.setValue(Rpn::FunctionMultiply);
	}
	else if(m_lexicalAnalyzer->lexeme().type == LexemeDivide) {
		result.value.setValue(Rpn::FunctionDivide);
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
	Rpn::CodeThread result;

	// first obligatory factor
	result << factor();

	// {MultOperation Factor} section
	while (CheckLexeme::isMultOperation(m_lexicalAnalyzer->lexeme())) {
		Rpn::Element operation = multOperation();
		result << factor() << operation;
	}

	return result;
}

// SummOperation = '+' | '-'
Rpn::Element SyntaxAnalyzer::summOperation()
{
	Rpn::Element result(Rpn::ElementFunctionCall);

	if (m_lexicalAnalyzer->lexeme().type == LexemePlus) {
		result.value.setValue(Rpn::FunctionPlus);
	}
	else if(m_lexicalAnalyzer->lexeme().type == LexemeMinus) {
		result.value.setValue(Rpn::FunctionMinus);
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
Rpn::Element SyntaxAnalyzer::constant()
{
	QString constName = m_lexicalAnalyzer->lexeme().value;
	m_lexicalAnalyzer->nextLexeme();

	// it is a formal argument
	Rpn::Argument possibleArgument(Rpn::OperandNumber, constName, QVariant());
	if (m_workingArguments.contains(possibleArgument)) {
		return Rpn::Element(Rpn::ElementArgument, constName);
	}
	
	// it is a constant
	else if (m_exprCalculator->isConstant(constName)) {
		return Rpn::Element(Rpn::ElementConstant, constName);
	}
	
	else {
		THROW(EUndeclaredUsed(constName, EUndeclaredUsed::Constant));
	}
}

// FormalArgument = Identifier
Rpn::Argument SyntaxAnalyzer::formalArgument()
{
	if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
		THROW(ELexemeExpected(tr("Argument name")));
	}
	
	QString argumentName = m_lexicalAnalyzer->lexeme().value;
	// user-defined functions are now can take only Numbers as arguments
	Rpn::Argument argument(Rpn::OperandNumber, argumentName);
	if (m_workingArguments.contains(argument)) {
		THROW(EFormalArgumentReused(argumentName));
	}
	m_lexicalAnalyzer->nextLexeme();

	return argument;
}

Rpn::CodeThread SyntaxAnalyzer::actualArgument(const Rpn::Argument &correspondingFormalArgument)
{
	switch (correspondingFormalArgument.type) {

		case Rpn::OperandNumber:
			return expression();

		case Rpn::OperandVector:
			return vector();

		case Rpn::OperandFunctionName: {
			if (m_lexicalAnalyzer->lexeme().type != LexemeIdentifier) {
				THROW(ELexemeExpected(tr("Function name")));
			}
			QString argumentFunctionName = m_lexicalAnalyzer->lexeme().value;

			if (!m_exprCalculator->isFunction(argumentFunctionName)) {
				THROW(EUndeclaredUsed(argumentFunctionName, EUndeclaredUsed::Function));
			}

			// check argument count if number of arguments is fixed
			if (correspondingFormalArgument.info.value<int>() != Rpn::ArbitraryArgumentsCount) {
				if (m_exprCalculator->functionArguments(argumentFunctionName).count()
					!= correspondingFormalArgument.info.value<int>()) {
					THROW(EIncorrectFunctionArgument(argumentFunctionName));
				}
			}

			// ensure all argumentFunctionName arguments are of Number type
			foreach (const Rpn::Argument &argument, m_exprCalculator->functionArguments(argumentFunctionName)) {
				if (argument.type != Rpn::OperandNumber) {
					THROW(EIncorrectFunctionArgument(argumentFunctionName));
				}
			}

			Rpn::Operand operand(Rpn::OperandFunctionName, argumentFunctionName);
			m_lexicalAnalyzer->nextLexeme();
			return Rpn::CodeThread() << Rpn::Element(Rpn::ElementOperand, QVariant::fromValue(operand));
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
