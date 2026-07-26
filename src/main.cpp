#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstring>
#include "FileHeader.hpp"
#include "ReadFile.hpp"

// Print file metadata
void printInfo(FileHeader& h) {
	std::cout << "RIFF chunk ID: " << std::string(h.ChunkID, 4) << std::endl;
	std::cout << "RIFF chunk size: " << h.ChunkSize << std::endl;
	std::cout << "RIFF Format: " << std::string(h.Format, 4) << std::endl;

		
	std::cout << "---------------"<< std::endl;

	std::cout << "fmt id: " << std::string(h.SubChunk1ID, 4) << std::endl;
	std::cout << "fmt size: " << h.SubChunk1Size << std::endl;
	std::cout << "fmt audio format: " << h.AudioFormat << std::endl;
	std::cout << "fmt Channels: " << h.NumChannels << std::endl;
	std::cout << "fmt Sample rate: " << h.SampleRate << std::endl;
	std::cout << "fmt byte range: " << h.ByteRate << std::endl;
	std::cout << "fmt Block align: " << h.BlockAlign << std::endl;
	std::cout << "fmt Bits per sample: " << h.BitsPerSample << std::endl;


	std::cout << "---------------"  << std::endl;

	std::cout << "Data id: " << std::string(h.SubChunk2ID, 4) << std::endl;
	// 29548544= ChunkSize - 44 (header) - 26 (skipped LIST chunk)
	std::cout << "Data size: " << h.SubChunk2Size << std::endl;
}

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
		printInfo(h);
	}

	return 0;
}
