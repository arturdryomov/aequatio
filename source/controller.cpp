#include "controller.h"
#include "mainwindow.h"
#include "QApplication"
#include "QTextCodec"



Controller *Controller::m_instance = 0;

Controller *Controller::instance()
{
	if (m_instance == 0) {
		m_instance = new Controller;
	}
	return m_instance;
}

int Controller::runApplication(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	// set codecs to unicode
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));	
	
	m_mainWindow = new MainWindow;
	connect(m_mainWindow, SIGNAL(commandEntered(QString)), SLOT(commandEntered(QString)));
	m_mainWindow->show();

	return a.exec();
}

void Controller::commandEntered(const QString &command)
{
	m_mainWindow->resultReturned(tr("I’ve got this command: ‘%1’").arg(command));
}

Controller::Controller(QObject *parent) :
    QObject(parent)
{
}

