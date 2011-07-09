#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QDateTime>
#include <QList>
#include <QFile>

struct Item {
	QString name;
	QString value;
};

struct LogItem {
	QDateTime time;
	QString name;
	QList<Item> addons;
};

const QString logFilename = "aequatio.log";

class Logger {
public:
	static Logger *instance();
	static void release();

	void add(LogItem item);
	void operator << (LogItem item);
protected:
	Logger();
	~Logger();
private:
	static Logger *m_instance;

	QFile m_logFile;

	void openFile();
};

#endif // LOGGER_H 
