#include "aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutWindow)
{
	ui->setupUi(this);
	ui->textLabel->setText(ui->textLabel->text().arg(qApp->applicationVersion()));

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

AboutWindow::~AboutWindow()
{
	delete ui;
}
