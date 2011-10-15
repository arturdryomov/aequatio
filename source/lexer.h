#ifndef LEXER_H
#define LEXER_H

#include "incorrectinputexceptions.h"

#include <QObject>
#include <QList>
#include <QString>
#include <QChar>
#include <QHash>

enum LexemeType {
	LexemeNumber,
	LexemePlus,
	LexemeMinus,
	LexemeMultiply,
	LexemeDivide,
	LexemeOpeningBracket,
	LexemeClosingBracket,
	LexemeOpeningSquareBracket,
	LexemeClosingSquareBracket,
	LexemePower,
	LexemeEqual,
	LexemeComma,
	LexemeIdentifier,
	LexemeConst,
	LexemeFunc,
	LexemeEol
};


struct Lexeme {
	LexemeType type;
	QString value;
};


class Lexer : public QObject
{
	Q_OBJECT

public:
	Lexer(QObject *parent = 0);
	~Lexer();

	void parse(const QString &input);
	Lexeme lexeme();
	void nextLexeme();
	void previousLexeme();

private:
	QString m_input;
	int m_position;
	QList<Lexeme> m_lexemeList;
	QListIterator<Lexeme> *m_lexemeListIterator;
	QHash<QString, LexemeType> m_reservedWords;

	void initializeReservedWords();
	void skipWhitespace();
	Lexeme endLexeme();
	void extractLexeme();
	void extractIdentifier();
	void extractNumber();
	void extractOperation();
	void extractBracket();
	void extractPower();
	void extractEqual();
	void extractComma();
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
	static bool isComma(QChar);
	static bool isDigit(QChar);
	static bool isLetter(QChar);
	static bool isUnderscore(QChar);
	static bool isLetterOrUnderscore(QChar);
	static bool isEqual(QChar);
	static bool isPlus(QChar);
	static bool isMinus(QChar);
	static bool isMultiply(QChar);
	static bool isDivide(QChar);
};


class EUnsupportedLexeme : public EInternal
{
public:
	EUnsupportedLexeme(const QString &unsupportedType);
	// User does't need to know what is unsupported here.
	// This info is for debug purpose only.
protected:
	LogItem logItem();
private:
	QString m_unsupported;
};


class EIncorrectCharacter : public EIncorrectInput
{
public:
	EIncorrectCharacter(QChar c);
	QString message();
private:
	QChar m_character;
};

#endif // LEXER_H
