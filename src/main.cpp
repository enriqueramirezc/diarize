#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstring>
#include "FileHeader.hpp"
#include "ReadHeader.hpp"
#include "ReadData.hpp"

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

	FileHeader h;
	ReadHeader in;

	if (in.fetchMetadata(h, ifs) == 1) {
		h.printHeaderInfo();
	}

	ReadData data;

	// Read file body
	if (data.fetchData(h, ifs) == 1) {
		std::cout << "bien" << std::endl;
	} else {
		std::cout << "mal" << std::endl;
	}

	return 0;
}
