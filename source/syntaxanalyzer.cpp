#include "syntaxanalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(QObject *parent) :
    QObject(parent)
{
}

QString SyntaxAnalyzer::parse(const QString &input)
{
	// stub
	return tr("I’ve got this command: ‘%1’").arg(input);
}
