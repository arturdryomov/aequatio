#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "calculator.h"
#include "document.h"
#include "codegenerator.h"
#include "incorrectinputexceptions.h"

#include <QObject>
#include <QString>
#include <QVector>

class Parser : public QObject
{
	Q_OBJECT

public:
	explicit Parser(QObject *parent = 0);
	~Parser();

	QString process(const QString &input, Document *document);

private:
	QString command();
	QString constDeclaration();
	QString functionDeclaration();
	Rpn::CodeThread expression();
	Rpn::CodeThread vector();
	Rpn::Vector extractVector();
	Rpn::CodeThread function();
	Rpn::CodeThread factor();
	Rpn::CodeThread powerBase();
	Rpn::CodeThread summand();
	Rpn::CodeThread identifier();
	BinaryOperation multOperation();
	BinaryOperation summOperation();
	Number number();

	QString formalArgument();
	void ensureNoMoreLexemes();
	QList<Rpn::Argument> functionArguments(const QString &functionName);

	QList<QString> m_workingArguments;
	Document *m_document;
	CodeGenerator *m_codeGenerator;
	Lexer *m_lexer;
	Calculator *m_calculator;
};

class CheckLexeme
{
public:
	static bool isMultOperation(Lexeme lexeme);
	static bool isSummOperation(Lexeme lexeme);
	static bool isUnaryOperation(Lexeme lexeme);
};

class EFormalArgumentReused : public EIncorrectInput
{
public:
	EFormalArgumentReused(const QString &argumentName);
	QString message();

private:
	QString m_argumentName;
};

class EIncorrectVectorInitialization : public EIncorrectInput
{
	QString message();
};


#endif // PARSER_H
