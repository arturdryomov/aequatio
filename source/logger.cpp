#include "logger.h"

#include <QTextStream>
#include <QDir>

// Singleton stuff

Logger *Logger::m_instance = 0;

Logger *Logger::instance()
{
  if (m_instance == 0) {
    m_instance = new Logger;
  }

  return m_instance;
}

void Logger::release()
{
	if (m_instance != 0) {
		delete m_instance;
	}
}

// Class code below

Logger::Logger()
{
	openFile();
}

Logger::~Logger()
{
	m_logFile.close();
}

void Logger::openFile()
{
	QString filePath = QDir::temp().filePath(logFilename);
	m_logFile.setFileName(filePath);

	if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return;
	}
}

void Logger::add(LogItem item)
{
	QTextStream out(&m_logFile);
	out << item.time.toString("hh:mm dd.MM.yyyy") << ": " << item.name << endl;

	foreach (Item addon, item.addons) {
		out << "\t" << addon.name << " = " << addon.value << endl;
	}
}

void Logger::operator << (LogItem item)
{
	add(item);
}
