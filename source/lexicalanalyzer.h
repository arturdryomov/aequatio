#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QChar>

enum LexemeType {
	LexemeNumber,
	LexemePlus,
	LexemeMinus,
	LexemeMultiply,
	LexemeDivide,
	LexemeOpeningBracket,
	LexemeClosingBracket,
	LexemePower,
	LexemeEOL
};

struct Lexeme {
	LexemeType type;
	QString value;
};

class LexicalAnalyzer : public QObject
{
    Q_OBJECT
public:
	LexicalAnalyzer(QObject *parent = 0);
	
	void parse(const QString &input);
	Lexeme lexeme();
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
	void extractBracket();
	void extractPower();
	void pushLexeme(LexemeType, QString);
};

class CheckChar
{
public:
	static bool isSeparator(QChar);
	static bool isSpace(QChar);
	static bool isExponent(QChar);
	static bool isOperation(QChar);
	static bool isSign(QChar);
	static bool isBracket(QChar);
	static bool isPower(QChar);
	static bool isDigit(QChar);
};

#endif // LEXICALANALYZER_H

