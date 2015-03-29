#include <iostream>
#include "../utility/apihook/apihook.h"

void main(int argc, char* argv[]) {
	if (argc != 4) {
		std::cout << "Usage: test.exe some.exe argument some.dll" << std::endl;
		exit(0);
	}
	try {
		utility::apihook::CreateProcessAndInject(argv[1], argv[2], argv[3]);
	} catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
		system("pause");
	}
}