#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <QList>
#include <QString>
#include <QChar>

enum LexemeType {
	lexemeNumber, lexemeEOL
};

struct Lexeme {
	LexemeType lexemeType;
	QString lexemeData;
};

class LexicalAnalyzer
{
public:
	LexicalAnalyzer(QString in) : m_input(in) {
		parseInput();
	}
	void parseInput();
	QString getLexeme();
	void nextLexeme();
private:
	QString m_input;
	int m_position;
	QList<Lexeme> m_lexemeList;
	void skipSymbols();
	void addEnd();
	void extractLexeme();
	void extractNumber();
	void pushLexeme(LexemeType, QString);
	class Exception
	{
	public:
		explicit Exception(const QString &message) : m_message(message) {}
		QString message() { return m_message; }
	private:
		QString m_message;
	};
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

