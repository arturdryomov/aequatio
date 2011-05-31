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
public slots:
	void resultReturned(const QString &result);
private slots:
	void submitButtonClicked();
protected:
	void resizeEvent(QResizeEvent *);
private:
	Ui::MainWindow *ui;	
};
	
#endif // MAINWINDOW_H
