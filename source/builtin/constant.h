#ifndef CONSTANT_H
#define CONSTANT_H

#include "../rpncode.h"

#include <QString>
#include <QHash>

namespace BuiltIn {

class Constant
{
public:
	Constant()
	{
	}

	static const QHash<QString, Number> &constants()
	{
		return constantsWritable();
	}

protected:
	// This method should be used for Constant children to add
	// constant-value pairs to the list. The addition should be done
	// in constructor.
	void addConstant(const QString &name, Number value)
	{
		// make sure 2 same-named constants are not added to the list
		Q_ASSERT(!constants().contains(name));

		constantsWritable().insert(name, value);
	}

private:
	static QHash<QString, Number> &constantsWritable()
	{
		static QHash<QString, Number> constants;
		return constants;
	}
};

} // namespace

#endif // CONSTANT_H
