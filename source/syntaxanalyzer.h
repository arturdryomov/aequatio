#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "lexicalanalyzer.h"
#include "exprcalculator.h"
#include "document.h"
#include "codegenerator.h"

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QHash>


enum ProcessingResultType {
	ResultExpressionCalculated,
	ResultConstantDeclared,
	ResultFunctionDeclared
};

struct ProcessingResult {
	ProcessingResultType type;
	QVariant data; // depends on type: Number, ConstantDescription or FunctionDescription respectively.
};

class SyntaxAnalyzer : public QObject
{
	Q_OBJECT

public:
	explicit SyntaxAnalyzer(QObject *parent = 0);
	~SyntaxAnalyzer();

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
	BinaryOperation multOperation();
	Rpn::CodeThread summand();
	BinaryOperation summOperation();
	Number number();
	Rpn::CodeThread constant();
	QString formalArgument();
	Rpn::CodeThread actualArgument(const Rpn::Argument &correspondingFormalArgument);
	void ensureNoMoreLexemes();
	QList<Rpn::Argument> functionArguments(const QString &functionName);
	
	CodeGenerator *m_codeGenerator;
	LexicalAnalyzer *m_lexicalAnalyzer;
	ExprCalculator *m_exprCalculator;
	QList<QString> m_workingArguments;
	Document *m_document;
};

class CheckLexeme
{
public:
	static bool isMultOperation(Lexeme lexeme);
	static bool isSummOperation(Lexeme lexeme);
	static bool isUnaryOperation(Lexeme lexeme);
};

#endif // SYNTAXANALYZER_H
