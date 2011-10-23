#include "newton.h"

namespace BuiltIn {
namespace EquationSolving {

namespace
{
	Newton instance;
}

Newton::Newton() :
	Function("newton")
{
}

Rpn::Operand Newton::calculate(FunctionCalculator *calculator, QList<Rpn::Operand> actualArguments)
{
	// Initialize algorithm variables
	m_calculator = calculator;
	m_functionName = actualArguments.at(0).value.value<QString>();
	m_searchInterval.leftBorder = actualArguments.at(1).value.value<Number>();
	m_searchInterval.rightBorder = actualArguments.at(2).value.value<Number>();
	m_accuracy = actualArguments.at(3).value.value<Number>();

	// Check condition of accuracy
	if (m_accuracy <= 0) {
		THROW(EWrongArgument(QObject::tr("accuracy"), QObject::tr("more than 0")) )
	}

	try {
		return Rpn::Operand(Rpn::OperandNumber, QVariant::fromValue(findSolution()));
	}
	catch (ENoSolution &e) {
		Q_UNUSED(e)
		return Rpn::Operand(Rpn::OperandIncorrect);
	}
}

QList<Rpn::Argument> Newton::requiredArguments()
{
	QList<Rpn::Argument> arguments;
	arguments
		// 1 is argument count in function that is passed as and argument
		<< Rpn::Argument(Rpn::OperandFunctionName, QString(), QVariant::fromValue(1))
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandNumber)
		<< Rpn::Argument(Rpn::OperandNumber);

	return arguments;
}

Rpn::OperandType Newton::returnValueType()
{
	return Rpn::OperandNumber;
}

Number Newton::findSolution()
{
	Number previousPoint = m_searchInterval.leftBorder +
		qAbs(m_searchInterval.rightBorder - m_searchInterval.leftBorder) / 2;
	Number currentPoint = previousPoint;

	do {
		previousPoint = currentPoint;

		// No solution if determinant is null
		if (MathUtils::isNull(countDerivative(previousPoint))) {
			THROW(ENoSolution());
		}

		currentPoint -= countFunction(previousPoint) / countDerivative(previousPoint);

		// No solution if point is out of range
		if ((currentPoint > m_searchInterval.rightBorder) || (currentPoint < m_searchInterval.leftBorder)) {
			THROW(ENoSolution());
		}

	} while (qAbs(currentPoint - previousPoint) >= m_accuracy);

	return currentPoint;
}

Number Newton::countFunction(Number argument)
{
	QList<Rpn::Operand> functionArguments;
	Rpn::Operand functionArgument(Rpn::OperandNumber, argument);
	functionArguments << functionArgument;

	Rpn::Operand result = m_calculator->calculate(m_functionName, functionArguments);
	return result.value.value<Number>();
}

Number Newton::countDerivative(Number argument)
{
	QList<Rpn::Operand> functionArguments;
	Rpn::Operand functionArgumentFunctionName(Rpn::OperandFunctionName, m_functionName);
	Rpn::Operand functionArgumentPoint(Rpn::OperandNumber, argument);
	functionArguments << functionArgumentFunctionName << functionArgumentPoint;

	Rpn::Operand result = m_calculator->calculate("derivative", functionArguments);
	return result.value.value<Number>();
}

} // namespace
} // namespace
