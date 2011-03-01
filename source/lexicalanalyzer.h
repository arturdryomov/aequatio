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

#endif // LEXICALANALYZER_H

