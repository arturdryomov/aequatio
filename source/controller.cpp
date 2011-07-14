#include "controller.h"
#include "mainwindow.h"
#include "application.h"
#include "exceptions.h"
#include "parsingexceptions.h"
#include "calculatingexceptions.h"

#include <QTextCodec>
#include <QProcess>
#include <QMessageBox>
#include <QFileInfo>

Controller *Controller::m_instance = 0;

Controller *Controller::instance()
{
	if (m_instance == 0) {
		m_instance = new Controller;
	}
	return m_instance;
}

void Controller::release()
{
	if (m_instance != 0) {
		delete m_instance;
	}	
}

int Controller::runApplication(int argc, char *argv[])
{
	Application a(argc, argv);
	
	// set codecs to unicode
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));	
	
	m_mainWindow = new MainWindow;
	connect(m_mainWindow, SIGNAL(commandEntered(QString)), SLOT(commandEntered(QString)));
	connect(m_mainWindow, SIGNAL(helpLaunched()), SLOT(helpLaunched()));
	m_mainWindow->show();
	
	int result = a.exec();
	delete m_mainWindow;
	return result;
}

void Controller::commandEntered(const QString &command)
{
	try {
		QString result = m_syntaxAnalyzer->process(command);
		m_mainWindow->resultReturned(result);		
	} 

	catch (EInternal &e) {
		m_mainWindow->resultReturned(e.message());
		e.toLogger();
	}
	catch (EParsing &e) {
		m_mainWindow->resultReturned(e.message());
	}
	catch (ECalculating &e) {
		m_mainWindow->resultReturned(e.message());
	}
}

void Controller::helpLaunched()
{
	// Check file
	if (!QFileInfo(helpFilename).exists()) {
		QMessageBox::information(m_mainWindow, "Aequatio", "Help is not available");
		return;
	}

	// Run Assistant
	QProcess *assistantProcess = new QProcess;
	QStringList assistantArgs;
	assistantArgs << QString("-collectionFile")
		<< helpFilename
		<< QString("-enableRemoteControl");
	assistantProcess->start(QString("assistant"), assistantArgs);
	if (!assistantProcess->waitForStarted()) {
		return;
	}

	// Send commands about appearance
	QByteArray assistantCommands;
	assistantCommands.append("hide bookmarks;");
	assistantCommands.append("setSource qthelp://aequatio/html/html/index.html\n");
	assistantProcess->write(assistantCommands);
}

Controller::Controller(QObject *parent) :
	QObject(parent),
	m_mainWindow(0),
m_syntaxAnalyzer(new SyntaxAnalyzer(this))
{
}

Controller::~Controller()
{
}
