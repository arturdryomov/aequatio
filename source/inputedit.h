#ifndef INPUTEDIT_H
#define INPUTEDIT_H

#include <QPlainTextEdit>

class InputEdit : public QPlainTextEdit
{
	Q_OBJECT
public:
	explicit InputEdit(QWidget *parent = 0);
signals:
	void returnPressed();
protected:
	void keyPressEvent(QKeyEvent *event);

};

#endif // INPUTEDIT_H
