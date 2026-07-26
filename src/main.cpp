#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstring>
#include "FileHeader.hpp"
#include "ReadFile.hpp"

int main() {

	// Read file name
	std::string input;
	std::cout << "Enter audio file name: " << std::endl;
	std::getline(std::cin, input);

	// Open file
	std::ifstream ifs(input, std::ios::binary);

	// Case where it couldn't be opened.
	if (!ifs.is_open()) {
		std::cout << "File " << input << " not found" << std::endl;
		return 0;
	}
	std::cout << "Processing " << input << " file" << std::endl;

	FileHeader h;
	ReadFile in;

	if (in.fetchMetadata(h, ifs) == 1) {
		h.printHeaderInfo();
	}

	return 0;
}
