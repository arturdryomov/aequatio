#include "inputedit.h"

InputEdit::InputEdit(QWidget *parent) :
    QPlainTextEdit(parent)
{
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
