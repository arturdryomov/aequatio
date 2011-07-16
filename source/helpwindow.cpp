#include "helpwindow.h"
#include "helpbrowser.h"

#include <QHelpEngine>
#include <QHelpContentItem>
#include <QFileInfo>
#include <QMessageBox>

HelpWindow::HelpWindow(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	initializeWindow();
}

HelpWindow::~HelpWindow()
{
}

void HelpWindow::initializeWindow()
{
	// Check help file for existing
	if (!QFileInfo(helpFilename).exists()) {
		QMessageBox::information(this, "Aequatio", "Help is not available");
		return;
	}

	// Try to load Help Engine
	QHelpEngine *helpEngine = new QHelpEngine(helpFilename, this);
	if (!helpEngine->setupData()) {
		QMessageBox::warning(this, "Aequatio", "Help is not available");
		delete helpEngine;
		return;
	}

	// Configure help window

	QSize windowSizes(600, 400);
	resize(windowSizes);

	HelpBrowser *helpBrowser = new HelpBrowser(helpEngine, this);
	QHelpContentWidget *helpWidget = helpEngine->contentWidget();

	QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
	splitter->addWidget(helpWidget);
	splitter->addWidget(helpBrowser);

	QList<int> splitterSizes;
	splitterSizes.append(windowSizes.width() / 3);
	splitterSizes.append(windowSizes.width() - splitterSizes.first());
	splitter->setSizes(splitterSizes);

	QGridLayout *layout = new QGridLayout(this) ;
	layout->addWidget(splitter);

	setWindowTitle("Aequatio Help");

	connect(helpWidget, SIGNAL(linkActivated(const QUrl &)), helpBrowser, SLOT(setSource(QUrl))) ;
}
