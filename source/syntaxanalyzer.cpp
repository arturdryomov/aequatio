#include "syntaxanalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(QObject *parent) :
    QObject(parent)
{
}

QString SyntaxAnalyzer::parse(const QString &input)
{
	// stub: any not-empty input is valid
	
	if (!input.isEmpty()) {
		return tr("I’ve got this command: ‘%1’").arg(input);	
	}
	else {
		throw Exception("Syntax parsing error");
	}
	
}

