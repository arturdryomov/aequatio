#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "lexicalanalyzer.h"
#include "exprcalculator.h"

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QHash>


enum ProcessingResultType {ResultExpressionCalculated, ResultConstantDeclared, ResultFunctionDeclared};
struct ProcessingResult {
	ProcessingResultType type;
	QVariant data; // depends on type, Number, ConstantDescription or FunctionDescription.
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
	RpnCodeThread expression();
	RpnCodeThread function();
	RpnCodeThread factor();
	RpnCodeThread powerBase();
	RpnElement multOperation();
	RpnCodeThread summand();	
	RpnElement summOperation();
	Number number();
	RpnElement constant();	
	void extractFormalArgument();
	void ensureNoMoreLexemes();
	
	LexicalAnalyzer *m_lexicalAnalyzer;
	ExprCalculator *m_exprCalculator;
	QList<QString> m_workingArguments;
};

class CheckLexeme
{
public:
	static bool isMultOperation(Lexeme lexeme);
	static bool isSummOperation(Lexeme lexeme);
	static bool isUnaryOperation(Lexeme lexeme);
};

#endif // SYNTAXANALYZER_H
