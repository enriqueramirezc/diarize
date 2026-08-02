#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstring>
#include "Wave.hpp"
#include "ReadFile.hpp"
#include "CSVWrite.hpp"

int main(int argc, char *argv[]) {

	if (argc != 3) {
		std::cout << "Must enter input file name" << std::endl;
		return 0;
	}

	// Read input file name
	std::string input = argv[1];

	// read output file name
	std::string output = argv[2];

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
	} else {
		std::cout << "Unable to read file metadata" << std::endl;
		return 0;
	}

	// Read file body
	if (in.fetchData(file, ifs) == 1) {
		std::cout << "Able to read file data" << std::endl;
	} else {
		std::cout << "Unable to read file data" << std::endl;
	}

	CSVWrite csv;

	if (csv.write(output, file.data.samples, file.format.SampleRate, file.format.NumChannels) == 1)  {
		std::cout << "Output successfully written to " << output << std::endl;
	} else {
		std::cout << "Unable to write output to " << output << std::endl;
	}

	return 0;
}
