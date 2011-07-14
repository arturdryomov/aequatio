#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
signals:
	void commandEntered(const QString &);	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void updateSidebar(const QString &contents);
public slots:
	void resultReturned(const QString &result);
private slots:
	void on_submitButton_clicked();
protected:
	void resizeEvent(QResizeEvent *);
private slots:
	void aboutTriggered();
private:
	Ui::MainWindow *ui;	
};

#endif // MAINWINDOW_H
