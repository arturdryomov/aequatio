#ifndef EXPRCALCULATOR_H
#define EXPRCALCULATOR_H

#include "rpncode.h"
#include "builtin/function.h"

#include <QHash>
#include <QObject>
#include <QMetaType>
#include <QList>

struct ConstantDescription {
	QString name;
	Number value;
};

struct FunctionDescription {
	QString name;
	QList<QString> arguments;
	QString body;
};

struct ExpressionDescription {
	QString expression;
    Rpn::Operand result;
};

// main function names
const QString FunctionMain = "@Main@";

class ExprCalculator : public QObject
{
	Q_OBJECT
signals:
	void constantsListChanged();
	void functionsListChanged();
public:
	explicit ExprCalculator(QObject *parent = 0);

    ExpressionDescription calculate(const Rpn::CodeThread &thread);
	ConstantDescription addConstant(const QString &name, const Number &value);
    FunctionDescription addFunction(const QString &name, const Rpn::Function &function);

	bool isFunction(const QString &name);
	bool isConstant(const QString &name);
    QList<Rpn::Argument> functionArguments(const QString &name);

	QList<ConstantDescription> constantsList();
	QList<FunctionDescription> functionsList();
private:
    QMap<QString, Rpn::Function> m_userDefinedFunctions;
	QMap<QString, Number> m_userDefinedConstants;

	friend class FunctionCalculator;
	class FunctionCalculator : public BuiltIn::Function::FunctionCalculator
	{
	public:
		FunctionCalculator(ExprCalculator *exprCalculator) : m_exprCalculator(exprCalculator) {}
        Rpn::Operand calculate(QString functionName, QList<Rpn::Operand> actualArguments);
        QList<Rpn::Argument> functionArguments(const QString &name);
	private:
		ExprCalculator *m_exprCalculator;
	};

	FunctionCalculator *m_functionCalculator;

	// PartPriority and PartInfo are used in codeThreadToString() only.
	enum PartPriority {PriorityPlusMinus, PriorityMultiplyDivide, PriorityPower,
		PriorityHighest, PriorityFunction = PriorityHighest, PriorityNumber = PriorityHighest};
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

    QString codeThreadToString(const Rpn::CodeThread &codeThread);

	// defines whether functionName is built-in or user-defined and calculates it.
    Rpn::Operand calculateFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments);
    Rpn::Operand calculateUserDefinedFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments);
    Rpn::Operand calculateBuiltInFunction(const QString &functionName, const QList<Rpn::Operand> &actualArguments);

	FunctionDescription functionDescription(const QString &functionName);
	// Checks, whether functionName is either called from code or from other functions that are called
	// from code.
    bool isFunctionUsed(const QString &functionName, const Rpn::CodeThread &code);
};

Q_DECLARE_METATYPE(ConstantDescription)
Q_DECLARE_METATYPE(FunctionDescription)
Q_DECLARE_METATYPE(ExpressionDescription)

#endif // EXPRCALCULATOR_H
