#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "mainwindow.h"
#include "syntaxanalyzer.h"



/* 
	This class interconnects ui level and application level.
	Controller implements Singletone pattern.
*/
class Controller : public QObject
{
	Q_OBJECT
public:
	static Controller *instance();	
	int runApplication(int argc, char *argv[]);
private slots:
	void commandEntered(const QString &command);
private:
	explicit Controller(QObject *parent = 0);	
	static Controller *m_instance;
	MainWindow *m_mainWindow;
	SyntaxAnalyzer *m_syntaxAnalyzer;
};

#endif // CONTROLLER_H
