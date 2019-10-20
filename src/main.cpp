#include "Controllers/StartController.h"

int main() {
	auto sController = std::make_shared<StartController>();
	if (!sController->init())
		return -1;
	
	return 0;
}

