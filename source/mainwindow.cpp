#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->consoleEdit->append(tr("→ You are welcome to Aequatio! Enter math expression, please…"));
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

void MainWindow::on_submitButton_clicked()
{
	// drop commandEdit text to console and send this text to external code
	
	QString command = ui->commandEdit->toPlainText().simplified();
	
	ui->consoleEdit->append("← " +command);
	emit commandEntered(command);
	ui->commandEdit->clear(); 
	ui->commandEdit->setFocus();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
	// scroll consoleEdit to the end
	ui->consoleEdit->verticalScrollBar()->setValue(ui->consoleEdit->verticalScrollBar()->maximum());
}
