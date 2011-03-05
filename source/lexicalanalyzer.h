#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <QList>
#include <QString>
#include <QChar>

enum LexemeType {
	lexemeNumber, lexemePlus, lexemeMinus,
	lexemeMultiply, lexemeDivide, lexemeEOL
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
	Lexeme getLexeme();
	void nextLexeme();
	class Exception
	{
	public:
		explicit Exception(const QString &message) : m_message(message) {}
		QString message() { return m_message; }
	private:
		QString m_message;
	};
private:
	QString m_input;
	int m_position;
	QList<Lexeme> m_lexemeList;
	void skipWhitespace();
	void addEnd();
	void extractLexeme();
	void extractNumber();
	void extractOperation();
	void pushLexeme(LexemeType, QString);
};

class CheckSymbol
{
public:
	static bool isSeparator(QChar);
	static bool isSpace(QChar);
	static bool isExponent(QChar);
	static bool isOperation(QChar);
	static bool isSign(QChar);
	static bool isDigit(QChar);
};

#endif // LEXICALANALYZER_H

