#include "helpwindow.h"
#include "helpbrowser.h"

#include <QHelpEngine>
#include <QHelpContentItem>
#include <QFileInfo>
#include <QMessageBox>

HelpWindow::HelpWindow(QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f),
	m_helpEngine(new QHelpEngine(qApp->applicationDirPath() + helpPath + helpFilename, this))
{
	m_helpEngine->setupData();

	QSize windowSizes(600, 400);
	resize(windowSizes);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	m_helpBrowser = new HelpBrowser(m_helpEngine, this);
	m_helpContentWidget = m_helpEngine->contentWidget();

	// We need to intercept single mouse click in order to show help topic at that time.
	// QHelpContentWidget::linkActivated() signal is emitted on double click which is inconvenient.
	// See also HelpWindow::eventFilter().
	m_helpContentWidget->viewport()->installEventFilter(this);

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
}

HelpWindow::~HelpWindow()
{
}

bool HelpWindow::eventFilter(QObject *watched, QEvent *event)
{
	// make sure we are filtering mouse event on help content widget
	if (event->type() != QEvent::MouseButtonRelease) {
		return false;
	}
	if (watched != m_helpContentWidget->viewport()) {
		return false;
	}

	// make sure this is a left button or middle button click
	QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);
	if (mouseEvent == 0) return false;
	if ((mouseEvent->button() != Qt::LeftButton) && (mouseEvent->button() != Qt::MiddleButton)) {
		return false;
	}

	// make sure the user pressed the button while the mouse was over one of the items
	const QModelIndex &modelIndex = m_helpContentWidget->indexAt(mouseEvent->pos());
	if (!modelIndex.isValid()) return false;

	QItemSelectionModel *selectionModel = m_helpContentWidget->selectionModel();
	if (selectionModel == 0) return false;

	if (!selectionModel->isSelected(modelIndex)) {
		return false;
	}

	// show the appropriate help topic
	QHelpContentModel *contentModel = qobject_cast<QHelpContentModel*>(m_helpContentWidget->model());
	if (contentModel != 0) {
		QHelpContentItem *item = contentModel->contentItemAt(modelIndex);
		if (item != 0) {
			m_helpBrowser->setSource(item->url());
		}
	}

	return true;
}

