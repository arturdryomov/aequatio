#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include "helpbrowser.h"

#include <QWidget>
#include <QtGui>

const QString helpFilename = "help.qhc";

#ifdef Q_WS_X11
	const QString helpPath = "/../share/doc/aequatio/";
#endif

#ifdef Q_WS_MAC
	const QString helpPath = "/../Resources/doc/";
#endif

#ifdef Q_WS_WIN
	const QString helpPath = "/help/";
#endif

class HelpWindow : public QDialog
{
	Q_OBJECT
public:
	HelpWindow(QWidget *parent = 0, Qt::WindowFlags f = 0);
	~HelpWindow();
protected:
	bool eventFilter(QObject *watched, QEvent *event);
private:
	QHelpEngine *m_helpEngine;
	QHelpContentWidget *m_helpContentWidget;
	HelpBrowser *m_helpBrowser;
};

#endif // HELPWINDOW_H
