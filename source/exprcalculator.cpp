#include "exprcalculator.h"
#include <QStack>

ExprCalculator::ExprCalculator(QObject *parent) :
    QObject(parent)
{
}

Number ExprCalculator::calculate(RpnCode *code)
{
	if (code == 0) {
		throw Exception(tr("There’s nothing to calculate"));
	}

	QStack<RpnElement> rpnStack;
	foreach (RpnElement element, code->elements) {
		if (element.type == RpnOperand) {
			rpnStack.push(element);
		}
		else {
			if (rpnStack.count() < 2) {
				// we cannot apply binary operation to less than two operands
				throw Exception(tr("At least two operators expected"));
			}
			
			RpnElement rightOperand = rpnStack.pop();
			RpnElement leftOperand = rpnStack.pop();
			OperationType operation = (OperationType)element.value.toInt();
			
			switch (operation) {
			case OperationMultiply:
				leftOperand.value = leftOperand.value.toDouble() * rightOperand.value.toDouble();
				break;
			case OperationDivide:
				leftOperand.value = leftOperand.value.toDouble() / rightOperand.value.toDouble();
				break;
			default:
				// there should'n be code here
				Q_ASSERT_X(false, "ExprCalculator", "Unknown operation in RPN code");
				break;
			}
			rpnStack.push(leftOperand);				
		}
	}
	
	if (rpnStack.count() != 1) {
		// there must be only one element in stack — the result
		throw Exception(tr("Expression is incorrect"));
	}
	
	if (rpnStack.top().type != RpnOperand) {
		// the result must be a number, not operation
		throw Exception(tr("Expression is incorrect"));
	}
	
	return rpnStack.pop().value.toDouble();
}
