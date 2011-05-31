#include "controller.h"

int main(int argc, char *argv[])
{		
	int result = Controller::instance()->runApplication(argc, argv);
	delete Controller::instance();
	
	return result;
}
