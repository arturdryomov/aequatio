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
		ProcessingResult result = m_syntaxAnalyzer->process(command);

		QString notificationText;

		switch (result.type) {

			case ResultExpressionCalculated: {
				ExpressionDescription exprpession = result.data.value<ExpressionDescription>();
				notificationText = QString("%1 = %2")
					.arg(exprpession.expression)
					.arg(numberToString(exprpession.result));
				break;
			}

			case ResultConstantDeclared: {
				ConstantDescription constant = result.data.value<ConstantDescription>();
				notificationText = tr("Constant declared: %1 = %2")
					.arg(constant.name)
					.arg(numberToString(constant.value));
				break;
			}

			case ResultFunctionDeclared: {
				FunctionDescription function = result.data.value<FunctionDescription>();
				notificationText = tr("Function declared: %1(%2) = %3")
					.arg(function.name)
					.arg(QStringList(function.arguments).join(", "))
					.arg(function.body);
				break;
			}
			default:
				THROW(EInternal());
		}

		m_mainWindow->resultReturned(notificationText);
	} 

	catch (EInternal &e) {
		m_mainWindow->displayErrorInfo(e.message());
		e.toLogger();
	}
	catch (EParsing &e) {
		m_mainWindow->displayErrorInfo(QString("<p><b>→ %1</b></p> <p>%2</p>").arg(command).arg(e.message()));
	}
	catch (ECalculating &e) {
		m_mainWindow->displayErrorInfo(QString("<p><b>%1</b></p> <p>%2</p>").arg(command).arg(e.message()));
	}
}

void Controller::constantsAndFunctionsUpdated()
{
	QList<ConstantDescription> constants = m_syntaxAnalyzer->constantsList();
	QList<FunctionDescription> functions = m_syntaxAnalyzer->functionsList();

	QString constantsText;
	foreach (ConstantDescription constant, constants) {
		constantsText += ListElementTemplate.arg(QString("%1 = %2")
				.arg(constant.name)
				.arg(numberToString(constant.value))
			);
	}
	if (constantsText.isEmpty()) {
		constantsText = ListElementTemplate.arg(tr("<i>&lt;nothing&gt;</i>"));
	}

	QString functionsText;
	foreach (FunctionDescription function, functions) {
		functionsText += ListElementTemplate.arg(
				QString("%1(%2) = %3")
				.arg(function.name)
				.arg(QStringList(function.arguments).join(", "))
				.arg(function.body)
			);
	}
	if (functionsText.isEmpty()) {
		functionsText = ListElementTemplate.arg(tr("<i>&lt;nothing&gt;</i>"));
	}

	QString fullText = ListHeaderTemplate.arg(tr("Constants")) + constantsText
		+ ListHeaderTemplate.arg(tr("Functions")) + functionsText;

	m_mainWindow->updateSidebar(fullText);
}

void Controller::launchHelp()
{
	if (!isHelpAvailable()) {
		QMessageBox::warning(m_mainWindow, "Aequatio", tr("Help is not available."));
		return;
	}

	if (m_helpWindow == 0) {
		m_helpWindow = new HelpWindow(m_mainWindow);
	}

	m_helpWindow->show();
	m_helpWindow->activateWindow();
}

bool Controller::isHelpAvailable()
{
	QHelpEngine helpEngine(helpFilename);
	return helpEngine.setupData();
}

void Controller::launchLogview()
{
	QString logPath = QDir::temp().filePath(logFilename);

	if (!isLogAvailable(logPath)) {
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

bool Controller::isLogAvailable(QString logPath)
{
	return QFileInfo(logPath).exists();
}

Controller::Controller(QObject *parent) :
	QObject(parent),
	m_mainWindow(0),
	m_helpWindow(0),
	m_logWindow(0),
	m_syntaxAnalyzer(new SyntaxAnalyzer(this))

{
	connect(m_syntaxAnalyzer, SIGNAL(constantsListChanged()), SLOT(constantsAndFunctionsUpdated()));
	connect(m_syntaxAnalyzer, SIGNAL(functionsListChanged()), SLOT(constantsAndFunctionsUpdated()));
}

Controller::~Controller()
{
}
