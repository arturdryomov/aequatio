#include "logwindow.h"

LogWindow::LogWindow(QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f)
{
	QSize windowSizes(500, 300);
	resize(windowSizes);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	m_textWidget = new QTextEdit(this);
	m_textWidget->setTextInteractionFlags(
		Qt::TextSelectableByMouse
		| Qt::TextSelectableByKeyboard
		| Qt::LinksAccessibleByMouse
		| Qt::LinksAccessibleByKeyboard);

	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(m_textWidget);

	setWindowTitle(tr("Debug Log"));
}

LogWindow::~LogWindow()
{
}

void LogWindow::setData(const QString &logContents)
{
	m_textWidget->setPlainText(logContents);
}
