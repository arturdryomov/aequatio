#include "mainwindow.h"
#include "QScrollBar"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
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

void MainWindow::on_commandEdit_returnPressed()
{
    // drop commandEdit text to console and send this text to external code
	ui->consoleEdit->append("← " + ui->commandEdit->text());
	emit commandEntered(ui->commandEdit->text());
	ui->commandEdit->clear();	
}

void MainWindow::resizeEvent(QResizeEvent *)
{
	// scroll consoleEdit to the end
	ui->consoleEdit->verticalScrollBar()->setValue(ui->consoleEdit->verticalScrollBar()->maximum());
}
