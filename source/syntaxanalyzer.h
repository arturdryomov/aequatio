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
signals:

public slots:

};

#endif // SYNTAXANALYZER_H
