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
		ProcessingResult result = m_syntaxAnalyzer->process(command);

		QString notificationText;

		switch (result.type) {

			case ResultExpressionCalculated: {
				CalculatingResult calculatingResult = result.data.value<CalculatingResult>();
				notificationText = QString("%1 = %2")
					.arg(calculatingResult.expression)
					.arg(calculatingResult.result);
				break;
			}

			case ResultConstDeclared: {
				ConstantDescription constant = result.data.value<ConstantDescription>();
				notificationText = tr("Constant declared: %1 = %2")
					.arg(constant.name)
					.arg(constant.value);
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
		constantsText += ListElementTemplate.arg(QString("%1 = %2").arg(constant.name, constant.value));
	}
	if (constantsText.isEmpty()) {
		constantsText = ListElementTemplate.arg(tr("&lt;nothing&gt;"));
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
		functionsText = ListElementTemplate.arg(tr("&lt;nothing&gt;"));
	}

	QString fullText = ListHeaderTemplate.arg(tr("Constants")) + constantsText
		+ ListHeaderTemplate.arg(tr("Functions")) + functionsText;

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
