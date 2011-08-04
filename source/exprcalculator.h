#ifndef EXPRCALCULATOR_H
#define EXPRCALCULATOR_H

#include "rpncode.h"
#include "builtinfunction.h"

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
	RpnOperand result;
};

// main function names
const QString RpnFunctionMain = "@Main@";

// built in constants
const QString Pi = "pi";
const QString E = "e";

class ExprCalculator : public QObject
{
	Q_OBJECT
signals:
	void constantsListChanged();
	void functionsListChanged();
public:
	explicit ExprCalculator(QObject *parent = 0);

	ExpressionDescription calculate(const RpnCodeThread &thread);
	ConstantDescription addConstant(const QString &name, const Number &value);
	FunctionDescription addFunction(const QString &name, const RpnFunction &function);

	bool isFunction(const QString &name);
	bool isConstant(const QString &name);
	QList<RpnArgument> functionArguments(const QString &name);

	QList<ConstantDescription> constantsList();
	QList<FunctionDescription> functionsList();
private:
	QList<QString> m_userDefinedFunctionNames; // stores function in order of their declaration
	QHash<QString, RpnFunction> m_userDefinedFunctions;
	QHash<QString, Number> m_userDefinedConstants;
	QHash<QString, Number> m_builtInConstants;

	friend class FunctionCalculator;
	class FunctionCalculator : public BuiltInFunction::FunctionCalculator
	{
	public:
		FunctionCalculator(ExprCalculator *exprCalculator) : m_exprCalculator(exprCalculator) {}
		RpnOperand calculate(QString functionName, QList<RpnOperand> actualArguments);
	private:
		ExprCalculator *m_exprCalculator;
	};

	FunctionCalculator *m_functionCalculator;

	// PartPriority and PartInfo are used in rpnCodeThreadToString() only.
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

	QString rpnCodeThreadToString(const RpnCodeThread &codeThread);
	void initializeBuiltInConstants();

	// defines whether functionName is built-in or user-defined and calculates it.
	RpnOperand calculateFunction(const QString &functionName, const QList<RpnOperand> &actualArguments);
	RpnOperand calculateUserDefinedFunction(const QString &functionName, const QList<RpnOperand> &actualArguments);
	RpnOperand calculateBuiltInFunction(const QString &functionName, const QList<RpnOperand> &actualArguments);

	FunctionDescription functionDescription(const QString &functionName);
};

Q_DECLARE_METATYPE(ConstantDescription)
Q_DECLARE_METATYPE(FunctionDescription)
Q_DECLARE_METATYPE(ExpressionDescription)

#endif // EXPRCALCULATOR_H
