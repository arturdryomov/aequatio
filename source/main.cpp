#include <QtGui>
#include "mainwindow.h"

int main(int argc, char *argv[])
{			
	QApplication a(argc, argv);
	
	// set codecs to unicode
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));	
	
	MainWindow w;
	w.show();

	return a.exec();
}
