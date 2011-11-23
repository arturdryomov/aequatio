#include "controller.h"
#include "mainwindow.h"
#include "application.h"
#include "exceptions.h"

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
	a.setApplicationVersion(QString("%1.%2.%3 %4")
		.arg(VERSION_MAJOR)
		.arg(VERSION_MINOR)
		.arg(VERSION_BUILD)
		.arg(versionRevisionFromNumber(VERSION_REVISION)));

	// Set codecs to unicode
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

	m_mainWindow = new MainWindow;
	connect(m_mainWindow, SIGNAL(commandEntered(QString)), SLOT(commandEntered(QString)));
	connect(m_mainWindow, SIGNAL(helpLaunchQueried()), SLOT(launchHelp()));
	connect(m_mainWindow, SIGNAL(logviewLaunch()), SLOT(launchLogview()));
	constantsAndFunctionsUpdated();
	m_mainWindow->show();

	int result = a.exec();
	delete m_mainWindow;
	return result;
}

void Controller::commandEntered(const QString &command)
{
	try {
		QString result = m_parser->process(command, m_document);
		m_mainWindow->resultReturned(result);
	}

	catch (EInternal &e) {
		m_mainWindow->displayErrorInfo(e.message());
		e.toLogger();
		return;
	}
	catch (EIncorrectInput &e) {
		m_mainWindow->displayErrorInfo(e.message());
		return;
	}

	m_mainWindow->clearInputArea();
}

void Controller::constantsAndFunctionsUpdated()
{
	// Prepare constants text
	QString constantsText;
	QStringList constants = m_document->prettyPrintedConstants();
	if (constants.isEmpty()) {
		constantsText = ListElementTemplate.arg(tr("<i>&lt;nothing&gt;</i>"));
	}
	else {
		foreach (const QString &constantText, constants) {
			constantsText += ListElementTemplate.arg(constantText);
		}
	}

	// Prepare functions text
	QString functionsText;
	QStringList functions = m_document->prettyPrintedFunctions();
	if (functions.isEmpty()) {
		functionsText = ListElementTemplate.arg(tr("<i>&lt;nothing&gt;</i>"));
	}
	else {
		foreach (const QString &functionText, functions) {
			functionsText += ListElementTemplate.arg(functionText);
		}
	}

	// Prepare full text and update sidebar
	QString fullText = ListHeaderTemplate.arg(tr("Constants")) + constantsText
		+ ListHeaderTemplate.arg(tr("Functions")) + functionsText;

	m_mainWindow->updateSidebar(fullText);
}

void Controller::launchHelp()
{
	QString helpFullPath = qApp->applicationDirPath() + helpPath + helpFilename;

	if (!QFileInfo(helpFullPath).exists()) {
		QMessageBox::information(m_mainWindow, "Aequatio", tr("Help is not available"));
		return;
	}

	if (!isHelpAvailable(helpFullPath)) {
		QMessageBox::warning(m_mainWindow, "Aequatio", tr("Help is not available."));
		return;
	}

	if (m_helpWindow == 0) {
		m_helpWindow = new HelpWindow(m_mainWindow);
	}

	m_helpWindow->show();
	m_helpWindow->activateWindow();
}

bool Controller::isHelpAvailable(QString helpFullPath)
{
	QHelpEngine helpEngine(helpFullPath);
	return helpEngine.setupData();
}

void Controller::launchLogview()
{
	QString logPath = QDir::temp().filePath(logFilename);

	if (!QFileInfo(logPath).exists()) {
		QMessageBox::information(m_mainWindow, "Aequatio", tr("Log is not available"));
		return;
	}

	if (m_logWindow == 0) {
		QFile logFile(logPath);
		if (!logFile.open(QIODevice::ReadOnly)) {
			QMessageBox::warning(m_mainWindow, "Aequatio", tr("Log is not available"));
			return;
		}

		m_logWindow = new LogWindow(m_mainWindow);
		QString logContents = logFile.readAll().constData();
		m_logWindow->setData(logContents);

		logFile.close();
	}

	m_logWindow->show();
	m_logWindow->activateWindow();
}

Controller::Controller(QObject *parent) :
	QObject(parent),
	m_mainWindow(0),
	m_helpWindow(0),
	m_logWindow(0),
	m_document(new Document(this)),
	m_parser(new Parser(this))

{
	connect(m_document, SIGNAL(constantsChanged()), SLOT(constantsAndFunctionsUpdated()));
	connect(m_document, SIGNAL(functionsChanged()), SLOT(constantsAndFunctionsUpdated()));
}

Controller::~Controller()
{
}

QString Controller::versionRevisionFromNumber(int number)
{
	if ((number > 9999) || (number < 1000))
		return QString();

	int firstNumber = number / 1000;
	int secondNumber = (number / 100) % 10;

	switch (firstNumber) {
		case 1: return "Dev";
		case 2: return QString("Pre %1").arg(secondNumber);
		case 3: return QString("Alpha %1").arg(secondNumber);
		case 4: return QString("Beta %1").arg(secondNumber);
		case 5: return QString("Release Candidate %1").arg(secondNumber);
		default: return QString("Final");
	}
}
