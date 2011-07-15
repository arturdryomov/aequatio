#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mainwindow.h"
#include "syntaxanalyzer.h"

#include <QObject>

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
private:
	explicit Controller(QObject *parent = 0);
	~Controller();
	
	static Controller *m_instance;
	MainWindow *m_mainWindow;
	SyntaxAnalyzer *m_syntaxAnalyzer;
};

#endif // CONTROLLER_H
