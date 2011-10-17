#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QTextBrowser>
#include <QtHelp/QHelpEngine>

// Get son of text browser
class HelpBrowser : public QTextBrowser
{
	Q_OBJECT

public:
	HelpBrowser(QHelpEngine *helpEngine, QWidget *parent = 0);

private:
	QHelpEngine	*m_helpEngine;

	QVariant loadResource(int type, const QUrl &name);
};

#endif // HELPBROWSER_H
