#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstring>
#include "Wave.hpp"
#include "ReadFile.hpp"

int main(int argc, char *argv[]) {

	if (argc != 2) {
		std::cout << "Must enter input file name" << std::endl;
		return 0;
	}

	// Read file name
	std::string input = argv[1];

	// Open file
	std::ifstream ifs(input, std::ios::binary);

	// Case where it couldn't be opened.
	if (!ifs.is_open()) {
		std::cout << "File " << input << " not found" << std::endl;
		return 0;
	}
	std::cout << "Processing " << input << " file" << std::endl;


	Wave file;
	ReadFile in;
	if (in.fetchMetadata(file, ifs) == 1) {
		file.printHeaderInfo();
	}

	// Read file body
	if (in.fetchData(file, ifs) == 1) {
		std::cout << "Able to read file data" << std::endl;
	} else {
		std::cout << "Unable to read file data" << std::endl;
	}

	return 0;
}
