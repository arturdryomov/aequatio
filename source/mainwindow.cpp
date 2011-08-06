#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"

#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->actionHelp, SIGNAL(triggered()), SIGNAL(helpLaunchQueried()));
	connect(ui->actionDebug_Log, SIGNAL(triggered()), SIGNAL(logviewLaunch()));
	connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

	setWindowIcon(QIcon(":/images/linux/aequatio.png"));
	ui->errorInformationLabel->hide();
	ui->actionShowFunctionsAndConstants->setChecked(false); // hide sidebar
	ui->consoleEdit->append(tr("→ You are welcome to Aequatio! Enter math expression, please…"));
	ui->consoleEdit->append(""); // new line
	ui->commandEdit->setFocus();	
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::resultReturned(const QString &result)
{	
	// drop result to console
	ui->consoleEdit->append("→ " + result);
}

void MainWindow::displayErrorInfo(const QString &info)
{
	ui->errorInformationLabel->setText(info);
	ui->errorInformationLabel->show();
}

void MainWindow::hideErrorInfo()
{
	ui->errorInformationLabel->hide();
}

void MainWindow::updateSidebar(const QString &contents)
{
	ui->functionsAndConstantsList->setHtml(contents);
}

void MainWindow::commandEntered()
{
	emit commandEntered(ui->commandEdit->text());
	ui->commandEdit->setFocus();
}

void MainWindow::clearInputArea()
{
	ui->commandEdit->clear();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
	// scroll consoleEdit to the end
	ui->consoleEdit->verticalScrollBar()->setValue(ui->consoleEdit->verticalScrollBar()->maximum());
}

void MainWindow::aboutTriggered()
{
	AboutWindow(this).exec();
}
