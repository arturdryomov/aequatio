#include "inputedit.h"

InputEdit::InputEdit(QWidget *parent) :
    QPlainTextEdit(parent)
{
	new Highlighter(this->document());
}

void InputEdit::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		if (event->modifiers().testFlag(Qt::ControlModifier)) {
			// append an empty new paragraph
			this->appendPlainText("");	
		}
		else {
			emit returnPressed();
		}
		return;
	}
	QPlainTextEdit::keyPressEvent(event);
}

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	// keywords
	rule.format.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp("\\bconst\\b");
	m_highlightingRules << rule;
	rule.pattern = QRegExp("\\bfunc\\b");
	m_highlightingRules << rule;

	// numbers
	rule.format.setForeground(Qt::blue);
	rule.pattern = QRegExp("\\b([0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?)\\b");
	m_highlightingRules << rule;
}

void Highlighter::highlightBlock(const QString &text)
{
	foreach (const HighlightingRule &rule, m_highlightingRules) {
		int start = rule.pattern.indexIn(text);
		while (start >= 0) {
			int length = rule.pattern.matchedLength();
			setFormat(start, length, rule.format);
			start = rule.pattern.indexIn(text, start + length);
		}
	}
}
