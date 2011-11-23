#ifndef PRETTYPRINTER_H
#define PRETTYPRINTER_H

#include "rpncode.h"
#include "document.h"

#include <QObject>

class PrettyPrinter
{

public:
	PrettyPrinter(const Document *document);

	void setDocument(const Document *document);
	QString process(const Rpn::CodeThread &codeThread);
	QString process(const Rpn::Function &function, const QString &functionName);

private:
	enum PartPriority {
		PriorityPlusMinus,
		PriorityMultiplyDivide,
		PriorityPower,
		PriorityHighest,
		PriorityFunction = PriorityHighest,
		PriorityNumber = PriorityHighest
	};

	struct PartInfo {
		QString text;
		PartPriority priority;

		void bracesIfGreater(PartPriority externalPriority) {
			if (externalPriority > priority) {
				text = QString("(%1)").arg(text);
			}
		}
		void bracesIfGreaterOrEqual(PartPriority externalPriority) {
			if (externalPriority >= priority) {
				text = QString("(%1)").arg(text);
			}
		}
	};

	const Document *m_document;
	QStack<PartInfo> m_codeParts;

	PartInfo constructFunctionPart(QString functionName);
};

#endif // PRETTYPRINTER_H
