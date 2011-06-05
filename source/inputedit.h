#ifndef INPUTEDIT_H
#define INPUTEDIT_H

#include <QPlainTextEdit>
#include <QSyntaxHighlighter>

class InputEdit : public QPlainTextEdit
{
	Q_OBJECT
public:
	explicit InputEdit(QWidget *parent = nullptr);
signals:
	void returnPressed();
protected:
	void keyPressEvent(QKeyEvent *event);

};

class Highlighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	Highlighter(QTextDocument *parent = nullptr);
protected:
	void highlightBlock(const QString &text);
private:
	struct HighlightingRule {
		QRegExp pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> m_highlightingRules;
};

#endif // INPUTEDIT_H
