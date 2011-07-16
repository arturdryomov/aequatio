#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include "helpbrowser.h"

#include <QWidget>
#include <QtGui>

const QString helpFilename = "help.qhc";

class HelpWindow : public QDialog
{
	Q_OBJECT
public:
	HelpWindow(QWidget *parent = 0, Qt::WindowFlags f = 0);
	~HelpWindow();
private:
	QHelpEngine *m_helpEngine;
};

#endif // HELPWINDOW_H
