#include "lexicalanalyzer.h"

QString LexicalAnalyzer::getLexeme()
{

	if (m_lexemeList.isEmpty()) {
		return QString();
	}
	QString tempLexeme = m_lexemeList.first();
	if (tempLexeme.isNull()) {
		return QString();
	}
	else {
		return tempLexeme;
	}

}

void LexicalAnalyzer::nextLexeme()
{
	m_lexemeList.removeFirst();
}

void LexicalAnalyzer::parseInput()
{
	if (m_input.isEmpty()) {
		return;
	}

	int inputLength = m_input.size() - 1;
	int position = 0;

	while (position <= inputLength) {
		int startPosition = position;

		skipSymbols(startPosition, position);

		if (!m_input.at(position).isNumber()) {
			return;
		}

		while ( (position < inputLength) && ((m_input.at(position).isNumber())) ) {
			position++;
		}

		if ( (position < inputLength) && (isSeparator(m_input.at(position))) ) {
			position++;
			if ( (position > inputLength) || (!m_input.at(position).isNumber()) ) {
				pushLexeme(startPosition, position);
			}
			while ( (position < inputLength) && (m_input.at(position).isNumber()) ) {
				position++;
			}

		}

		pushLexeme(startPosition, position);
	}

	addEnd();
}

void LexicalAnalyzer::skipSymbols(int &startPosition, int &position)
{
	QList<QChar> symbols;
	symbols << ' ';
	while (position < m_input.size() - 1) {
		if (symbols.contains(m_input.at(position))) {
			position++;
			startPosition++;
		}
		else {
			return;
		}
	}
}

void LexicalAnalyzer::pushLexeme(int startPosition, int endPosition)
{
	QString tempNumber;

	while (startPosition < endPosition) {
		tempNumber.append(m_input.at(startPosition));
		startPosition++;
	}

	if (!tempNumber.isNull()) {
		m_lexemeList.append(tempNumber);
	}
}

void LexicalAnalyzer::addEnd()
{
	QString end;
	m_lexemeList.append(end);
}

bool LexicalAnalyzer::isSeparator(QChar symbol)
{
	QList<QChar> separators;
	separators << '.';
	return separators.contains(symbol);
}
