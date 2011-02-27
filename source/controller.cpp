#include "controller.h"
#include "mainwindow.h"
#include "application.h"
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
	Application a(argc, argv);
	
	// set codecs to unicode
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));	
	
	m_mainWindow = new MainWindow;
	connect(m_mainWindow, SIGNAL(commandEntered(QString)), SLOT(commandEntered(QString)));
	m_mainWindow->show();
	
	int result = a.exec();
	delete m_mainWindow;
	return result;
}

void Controller::commandEntered(const QString &command)
{
	try {
		QString result = m_syntaxAnalyzer->parse(command);
		m_mainWindow->resultReturned(result);		
	} 
	catch (SyntaxAnalyzer::Exception e) {
		m_mainWindow->resultReturned(e.message());
	}
		
}

Controller::Controller(QObject *parent) :
	QObject(parent),
	m_mainWindow(0),
	m_syntaxAnalyzer(new SyntaxAnalyzer(this))
{
}

