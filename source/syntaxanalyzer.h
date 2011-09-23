#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "lexicalanalyzer.h"
#include "exprcalculator.h"

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
signals:
	void constantsListChanged();
	void functionsListChanged();
public:
	explicit SyntaxAnalyzer(QObject *parent = 0);
	~SyntaxAnalyzer();

	ProcessingResult process(const QString &input);
	QList<ConstantDescription> constantsList();
	QList<FunctionDescription> functionsList();
private:
	ProcessingResult command();
	ConstantDescription constDeclaration();
	FunctionDescription functionDeclaration();
    Rpn::CodeThread expression();
    Rpn::CodeThread vector();
    Rpn::Vector extractVector();
    Rpn::CodeThread function();
    Rpn::CodeThread factor();
    Rpn::CodeThread powerBase();
    Rpn::Element multOperation();
    Rpn::CodeThread summand();
    Rpn::Element summOperation();
	Number number();
    Rpn::Element constant();
    Rpn::Argument formalArgument();
    Rpn::CodeThread actualArgument(const Rpn::Argument &correspondingFormalArgument);
	void ensureNoMoreLexemes();
	
	LexicalAnalyzer *m_lexicalAnalyzer;
	ExprCalculator *m_exprCalculator;
    QList<Rpn::Argument> m_workingArguments;
};

class CheckLexeme
{
public:
	static bool isMultOperation(Lexeme lexeme);
	static bool isSummOperation(Lexeme lexeme);
	static bool isUnaryOperation(Lexeme lexeme);
};

#endif // SYNTAXANALYZER_H
