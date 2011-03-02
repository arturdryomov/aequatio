#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <QList>
#include <QString>
#include <QChar>

struct lexeme {
	QString lexemeData;
	QString lexemeType;
};

class LexicalAnalyzer
{
public:
	LexicalAnalyzer(QString in) : m_input(in)
		{ }
	void parseInput();
	QString getLexeme();
	void nextLexeme();
private:
	QString m_input;
	int m_position;
	QList<lexeme> m_lexemeList;
	void skipSymbols(int &startPosition);
	void addEnd();
	void pushNumber(int startPosition);
	void extractLexeme(int startPosition);
	void extractNumber(int &startPosition);
};

class CheckSymbol
{
public:
	static bool isSeparator(QChar symbol);
	static bool isSpace(QChar symbol);
	static bool isExponent(QChar symbol);
	static bool isOperation(QChar symbol);
	static bool isSign(QChar symbol);
	static bool isDigit(QChar symbol);
};

#endif // LEXICALANALYZER_H

