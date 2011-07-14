#include "controller.h"
#include "mainwindow.h"
#include "application.h"
#include "exceptions.h"
#include "parsingexceptions.h"
#include "calculatingexceptions.h"

#include <QTextCodec>

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
	constantsAndFunctionsUpdated();
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

void Controller::constantsAndFunctionsUpdated()
{
	QList<ConstantDescription> constants = m_syntaxAnalyzer->constantsList();
	QList<FunctionDescription> functions = m_syntaxAnalyzer->functionsList();

	QString constantsText;
	foreach (ConstantDescription constant, constants) {
		constantsText += QString("<p> %1 = %2 </p>").arg(constant.name).arg(constant.value);
	}
	if (constantsText.isEmpty()) {
		constantsText = tr("<i> &lt;nothing&gt; </i>");
	}

	QString functionsText;
	foreach (FunctionDescription function, functions) {
		functionsText += QString("<p> %1(%2) = %3 </p>")
			.arg(function.name)
			.arg(QStringList(function.arguments).join(", "))
			.arg(function.body);
	}
	if (functionsText.isEmpty()) {
		functionsText = tr("<i> &lt;nothing&gt; </i>");
	}

	QString fullText = QString("<h1>Constants</h1> %1 <h1>Functions</h1> %2")
		.arg(constantsText, functionsText);

	m_mainWindow->updateSidebar(fullText);
}

Controller::Controller(QObject *parent) :
	QObject(parent),
	m_mainWindow(0),
	m_syntaxAnalyzer(new SyntaxAnalyzer(this))
{
	connect(m_syntaxAnalyzer, SIGNAL(constantsListChanged()), SLOT(constantsAndFunctionsUpdated()));
	connect(m_syntaxAnalyzer, SIGNAL(functionsListChanged()), SLOT(constantsAndFunctionsUpdated()));
}

Controller::~Controller()
{
}
