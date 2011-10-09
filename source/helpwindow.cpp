#include "helpwindow.h"
#include "helpbrowser.h"

#include <QHelpEngine>
#include <QHelpContentItem>
#include <QFileInfo>
#include <QMessageBox>
#include <QTimer>

HelpWindow::HelpWindow(QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f),
	m_helpEngine(new QHelpEngine(qApp->applicationDirPath() + helpPath + helpFilename, this))
{
	m_helpEngine->setupData();

	QSize windowSizes(600, 400);
	resize(windowSizes);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::WindowMaximizeButtonHint);

	m_helpBrowser = new HelpBrowser(m_helpEngine, this);
	m_helpContentWidget = m_helpEngine->contentWidget();

	// We need to intercept single mouse click in order to show help topic at that time.
	// QHelpContentWidget::linkActivated() signal is by default emitted on double click which is inconvenient.
	// See also HelpWindow::eventFilter().
	m_helpContentWidget->viewport()->installEventFilter(this); // MouseButtonRelease event comes to viewport
	m_helpContentWidget->installEventFilter(this); // KeyPress event comes to QHelpContentWidget itself

	connect(m_helpBrowser, SIGNAL(sourceChanged(QUrl)), SLOT(onHelpBrowserSourceChanged(QUrl)));

	QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
	splitter->addWidget(m_helpContentWidget);
	splitter->addWidget(m_helpBrowser);

	QList<int> splitterSizes;
	splitterSizes.append(windowSizes.width() / 3);
	splitterSizes.append(windowSizes.width() - splitterSizes.first());
	splitter->setSizes(splitterSizes);

	QGridLayout *layout = new QGridLayout(this) ;
	layout->addWidget(splitter);

	setWindowTitle(tr("Aequatio Help"));

	// calling activateFirstTopLevel() directly leads to nothing -- it seems m_helpEngine
	// has not already been populated at that time.
	QTimer::singleShot(0, this, SLOT(activateFirstTopLevel()));
}

HelpWindow::~HelpWindow()
{
}

bool HelpWindow::eventFilter(QObject *watched, QEvent *event)
{
	// we are filtering mouse event on help content widget's viewport
	if (watched == m_helpContentWidget->viewport()) {
		if (event->type() != QEvent::MouseButtonRelease) {
			return false;
		}
		return processMouseEvent(dynamic_cast<QMouseEvent *>(event));
	}

	// we are filtering mouse event on help content widget
	if (watched == m_helpContentWidget) {
		if (event->type() != QEvent::KeyPress) {
			return false;
		}
		return processKeyEvent(dynamic_cast<QKeyEvent *>(event));
	}

	return false;
}

// returns true if the event is succesfully processed
bool HelpWindow::processKeyEvent(QKeyEvent *event)
{
	Q_UNUSED(event)
	if (event->key() != Qt::Key_Return) return false;

	showHelpForCurrentItem();
	return true;
}

// returns true if the event is succesfully processed
bool HelpWindow::processMouseEvent(QMouseEvent *event)
{
	// make sure this is a left button or middle button click
	if (event == 0) return false;
	if ((event->button() != Qt::LeftButton) && (event->button() != Qt::MiddleButton)) {
		return false;
	}

	// make sure the user pressed the button while the mouse was over one of the items
	const QModelIndex &modelIndex = m_helpContentWidget->indexAt(event->pos());
	if (!modelIndex.isValid()) return false;

	// show the appropriate help topic
	showHelpForCurrentItem();

	return true;
}

void HelpWindow::showHelpForCurrentItem()
{
	QHelpContentModel *contentModel = qobject_cast<QHelpContentModel*>(m_helpContentWidget->model());
	if (contentModel == 0) return;

	if (m_helpContentWidget->currentIndex() == m_helpContentWidget->rootIndex()) {
		return;
	}

	QHelpContentItem *item = contentModel->contentItemAt(m_helpContentWidget->currentIndex());
	if (item == 0) return;

	QString t = item->url().toString();

	// do not update if it is already set
	QUrl currentSource = m_helpBrowser->source();
	if (currentSource.isValid()) {
		if (m_helpBrowser->source() == item->url()) return;
	}

	m_helpBrowser->setSource(item->url());
	m_helpBrowser->setFocus();
}

void HelpWindow::activateFirstTopLevel()
{
	QModelIndex helpRoot = m_helpEngine->contentModel()->index(0, 0);
	if (!helpRoot.isValid()) return;

	m_helpContentWidget->expand(helpRoot);
	m_helpContentWidget->setCurrentIndex(helpRoot);
	showHelpForCurrentItem();
}

void HelpWindow::onHelpBrowserSourceChanged(const QUrl &src)
{
	// synchronize with help content widget
	QModelIndex currentIndex = m_helpContentWidget->currentIndex();
	QModelIndex indexOfSource = m_helpContentWidget->indexOf(src);
	if (currentIndex != indexOfSource) {
		m_helpContentWidget->setCurrentIndex(indexOfSource);
	}
}
