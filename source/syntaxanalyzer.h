#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include <QObject>
#include <QString>

class SyntaxAnalyzer : public QObject
{
    Q_OBJECT
public:
	explicit SyntaxAnalyzer(QObject *parent = 0);
	QString parse(const QString &input);
	
	// This exception will be thrown if parsing or computing error occurs.
	// message() returns error description.
	class Exception 
	{
	public:
		explicit Exception(const QString &message) : m_message(message) {}
		QString message() { return m_message; }
	private:
		QString m_message;
	};

signals:

public slots:

};

#endif // SYNTAXANALYZER_H
