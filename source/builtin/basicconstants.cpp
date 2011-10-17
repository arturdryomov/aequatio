#include "basicconstants.h"

namespace BuiltIn {
namespace BasicConstants {

namespace {
	BasicConstants instance;
}

BasicConstants::BasicConstants()
{
	this->addConstant("pi", M_PI);
	this->addConstant("e", M_E);
}

}
}

