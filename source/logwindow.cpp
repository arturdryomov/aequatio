#include "logwindow.h"

LogWindow::LogWindow(QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f)
{
	QSize windowSizes(500, 300);
	resize(windowSizes);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	m_textWidget = new QTextEdit(this);

	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(m_textWidget);

	setWindowTitle(tr("Aequatio Debug Log"));
}

LogWindow::~LogWindow()
{
}

void LogWindow::setData(QString logContents)
{
	m_textWidget->setPlainText(logContents);
}
