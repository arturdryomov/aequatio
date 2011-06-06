#include "compilerspecific.h"
#include "controller.h"

int main(int argc, char *argv[])
{		
	int result = Controller::instance()->runApplication(argc, argv);
	Controller::release();
	
	return result;
}
