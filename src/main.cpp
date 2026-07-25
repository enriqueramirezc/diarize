#include <iostream>
#include <fstream>
#include <string>

int main() {

	std::string input;

	std::getline(std::cin, input);

	// Open `.wav` file
	std::ifstream ifs;
	ifs.open(input, std::ios::binary);

	// Check
	if (ifs.is_open()) {
		std::cout<< "Good" <<std::endl;
		int length = 1024;
		char* buffer = new char[length];
		ifs.read(buffer, length);
	} else {
		std::cout<< "Bad" << std::endl;
		std::cout << "File " << input << " not found" << std::endl;
	}
	return 0;
}

