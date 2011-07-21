#include <QtGui>
#include <QSplitter>
#include "helpbrowser.h"
#include <QtHelp/QHelpEngine>

HelpBrowser::HelpBrowser(QHelpEngine *helpEngine, QWidget *parent) :
	QTextBrowser(parent), m_helpEngine(helpEngine)
{
}

QVariant HelpBrowser::loadResource(int type, const QUrl &name)
{
	if (!m_helpEngine) {
		return QVariant();
	}
	// URL
	if (name.scheme() == "qthelp") {
		return m_helpEngine->fileData(name);
	}
	// Just file
	else {
		return QTextBrowser::loadResource(type, name);
	}
}
