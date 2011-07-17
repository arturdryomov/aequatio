#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mainwindow.h"
#include "helpwindow.h"
#include "logwindow.h"
#include "syntaxanalyzer.h"

#include <QObject>

const QString ShowLogParam = "--showlog";

/* 
	This class interconnects ui level and application level.
	Controller implements Singletone pattern.
*/
class Controller : public QObject
{
	Q_OBJECT
public:
	static Controller *instance();
	static void release();
	
	int runApplication(int argc, char *argv[]);
private slots:
	void commandEntered(const QString &command);
	void constantsAndFunctionsUpdated();
	void launchHelp();
	void launchLogview();
private:
	explicit Controller(QObject *parent = 0);
	~Controller();
	
	static Controller *m_instance;
	MainWindow *m_mainWindow;
	HelpWindow *m_helpWindow;
	LogWindow *m_logWindow;
	QHelpEngine *m_helpEngine;
	SyntaxAnalyzer *m_syntaxAnalyzer;

	bool isHelpAvailable();
};

#endif // CONTROLLER_H
