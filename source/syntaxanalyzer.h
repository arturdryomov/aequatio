#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QList>
#include "lexicalanalyzer.h"
#include "exprcalculator.h"

class SyntaxAnalyzer : public QObject
{
	Q_OBJECT
public:
	explicit SyntaxAnalyzer(QObject *parent = 0);
	~SyntaxAnalyzer();
	
	QString process(const QString &input);
	
	// This exception will be thrown if parsing error occurs.
	// message() returns error description.
	class Exception 
	{
	public:
		explicit Exception(const QString &message) : m_message(message) {}
		QString message() { return m_message; }
	private:
		QString m_message;
	};

private:
	void expression(); // Expression = Summand {SummOperator Summand}
	RpnCodeThread factor(); // Factor = Number
	RpnElement multOperation(); // MultOperation = '*' | '/'
	RpnCodeThread summand(); // Summand = Factor {MultOperator Factor}
	RpnElement summOperation(); // SummOperation = '+' | '-'

	RpnCode *m_rpnCode;
	LexicalAnalyzer *m_lexicalAnalyzer;	
	ExprCalculator *m_exprCalculator;
};

class CheckLexeme
{
public:
	static bool isMultOperation(Lexeme lexeme);
	static bool isSummOperation(Lexeme lexeme);
};


#endif // SYNTAXANALYZER_H
