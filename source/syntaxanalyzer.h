#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "lexicalanalyzer.h"
#include "exprcalculator.h"

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QHash>

class SyntaxAnalyzer : public QObject
{
	Q_OBJECT
public:
	explicit SyntaxAnalyzer(QObject *parent = nullptr);
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
	QString command();
	QString constDeclaration();
	QString functionDeclaration();
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
	QVector<QString> m_workingParams;
};

class CheckLexeme
{
public:
	static bool isMultOperation(Lexeme lexeme);
	static bool isSummOperation(Lexeme lexeme);
	static bool isUnaryOperation(Lexeme lexeme);
};

#endif // SYNTAXANALYZER_H
