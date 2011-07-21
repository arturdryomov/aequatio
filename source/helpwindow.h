#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include "helpbrowser.h"

#include <QWidget>
#include <QtGui>

#ifdef Q_WS_X11
	const QString helpFilename = "../share/doc/aequatio/help.qhc";
#else
	const QString helpFilename = "help.qhc";
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
