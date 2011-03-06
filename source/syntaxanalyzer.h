#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QList>
#include "lexicalanalyzer.h"

typedef double Number;

enum RpnElementType { RpnOperation, RpnOperand };
enum OperationType { OperationPlus, OperationMinus, OperationMultiply, 
	OperationDivide };

struct RpnElement // RPN stands for ‘reverse Polish notation’
{
	RpnElementType type;
	QVariant value; // this will hold a Number or a OperationType value
};

struct RpnCode
{
	QList<RpnElement> elements; // there will be a number of lists here: main thread, functions…
};

class SyntaxAnalyzer : public QObject
{
    Q_OBJECT
public:
	explicit SyntaxAnalyzer(QObject *parent = 0);
	~SyntaxAnalyzer();
	
	QString process(const QString &input);
	
	// This exception will be thrown if parsing or computing error occurs.
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
	void expression(); // Expression = Factor {FactorOp Factor}
	RpnElement factor(); // Factor = Number
	RpnElement factorOp(); // FactorOp = '*' | '/'
	
	RpnCode *m_rpnCode;
	LexicalAnalyzer *m_lexicalAnalyzer;	
};

class CheckLexeme
{
public:
	static bool isFactorOp(Lexeme lexeme);
};


#endif // SYNTAXANALYZER_H
