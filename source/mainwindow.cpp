#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::resultReturned(const QString &result)
{	
	// drop result to console
	ui->consoleEdit->append("> " + result);
}

void MainWindow::on_commandEdit_returnPressed()
{
    // drop commandEdit text to console and send this text to external code
	ui->consoleEdit->append("← " + ui->commandEdit->text());
	emit commandEntered(ui->commandEdit->text());
	ui->commandEdit->clear();	
}
