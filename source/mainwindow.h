#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

static const QString ListHeaderTemplate = "<h3>%1</h3>";
static const QString ListElementTemplate = "<p style='margin: 0px 0px 4px 20px'> %1 </p>";

class MainWindow : public QMainWindow
{
	Q_OBJECT
signals:
	void commandEntered(const QString &);
	void helpLaunchQueried();
	void logviewLaunch();
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
public slots:
	void displayErrorInfo(const QString &info);
	void hideErrorInfo();
	void updateSidebar(const QString &contents);
	void resultReturned(const QString &result);
private slots:
	void commandEntered();
protected:
	void resizeEvent(QResizeEvent *);
private slots:
	void aboutTriggered();
private:
	Ui::MainWindow *ui;	
};

#endif // MAINWINDOW_H
