#include "rpncode.h"

bool operator ==(const RpnArgument &a1, const RpnArgument &a2)
{
	return (a1.name == a2.name)
		&& (a1.type == a2.type)
		&& (a1.info == a2.info);
}
