#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <QList>
#include <QString>
#include <QChar>

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
	QList<QString> m_lexemeList;
	bool isSeparator(QChar symbol);
	void skipSymbols(int &startPosition, int &position);
	void addEnd();
	void pushLexeme(int startPosition, int endPosition);
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

