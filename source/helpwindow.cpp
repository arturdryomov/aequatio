#include "helpwindow.h"
#include "helpbrowser.h"

#include <QHelpEngine>
#include <QHelpContentItem>
#include <QFileInfo>
#include <QMessageBox>

HelpWindow::HelpWindow(QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f),
	m_helpEngine(new QHelpEngine(helpFilename, this))
{
	m_helpEngine->setupData();

	QSize windowSizes(600, 400);
	resize(windowSizes);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	HelpBrowser *helpBrowser = new HelpBrowser(m_helpEngine, this);
	QHelpContentWidget *helpWidget = m_helpEngine->contentWidget();

	QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
	splitter->addWidget(helpWidget);
	splitter->addWidget(helpBrowser);

	QList<int> splitterSizes;
	splitterSizes.append(windowSizes.width() / 3);
	splitterSizes.append(windowSizes.width() - splitterSizes.first());
	splitter->setSizes(splitterSizes);

	QGridLayout *layout = new QGridLayout(this) ;
	layout->addWidget(splitter);

	setWindowTitle(tr("Aequatio Help"));

	connect(helpWidget, SIGNAL(linkActivated(const QUrl &)), helpBrowser, SLOT(setSource(QUrl))) ;
}

HelpWindow::~HelpWindow()
{
}

