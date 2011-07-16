#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QWidget>
#include <QtGui>

class LogWindow : public QDialog
{
	Q_OBJECT
public:
	LogWindow(QWidget *parent = 0, Qt::WindowFlags f = 0);
	~LogWindow();

	void setData(QString logContents);
private:
	QTextEdit *m_textWidget;
};

#endif // LOGWINDOW_H
