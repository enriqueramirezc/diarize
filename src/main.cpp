#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstring>
#include "FileHeader.hpp"

int main() {

	// Read file name
	std::string input;
	std::cout << "Enter audio file name: " << std::endl;
	std::getline(std::cin, input);

	// Open file
	std::ifstream ifs(input, std::ios::binary);

	// Case where it couldn't be opened.
	if (!ifs.is_open()) {
		std::cout << "Bad" << std::endl;
		std::cout << "File " << input << " not found" << std::endl;
		return 0;
	}
	std::cout << "Good" << std::endl;

	FileHeader h;

	// RIFF chunk descriptor (always first 12 bytes)
	ifs.read(h.ChunkID, 4);
	ifs.read(reinterpret_cast<char*>(&h.ChunkSize), 4);
	ifs.read(h.Format, 4);

	// check it's a valid file
	if (std::string(h.ChunkID, 4) != "RIFF" || std::string(h.Format, 4) != "WAVE") {
		std::cout << "Not a valid WAV file" << std::endl;
		return 0;
	}

	bool foundFmt = false;
	bool foundData = false;

	// Walk chunks until we find "data" chunk header
	while (ifs && !foundData) {
		char chunkID[4];
		uint32_t chunkSize;
		ifs.read(chunkID, 4);
		ifs.read(reinterpret_cast<char*>(&chunkSize), 4);
		if (!ifs) break;

		// if we find format chunk marker...
		if (std::string(chunkID, 4) == "fmt ") {
			std::memcpy(h.SubChunk1ID, chunkID, 4);
			h.SubChunk1Size = chunkSize;
			ifs.read(reinterpret_cast<char*>(&h.AudioFormat), 2);
			ifs.read(reinterpret_cast<char*>(&h.NumChannels), 2);
			ifs.read(reinterpret_cast<char*>(&h.SampleRate), 4);
			ifs.read(reinterpret_cast<char*>(&h.ByteRate), 4);
			ifs.read(reinterpret_cast<char*>(&h.BlockAlign), 2);
			ifs.read(reinterpret_cast<char*>(&h.BitsPerSample), 2);
			// fmt chunk may be larger than 16. Skip extra bytes
			if (chunkSize > 16) {
				ifs.seekg(chunkSize - 16, std::ios::cur);
			}
			foundFmt = true;

		// found data header
		} else if (std::string(chunkID, 4) == "data") {
			std::memcpy(h.SubChunk2ID, chunkID, 4);
			h.SubChunk2Size = chunkSize;
			foundData = true;
		} else {
			// skip other chunks (e.g. LIST)...
			ifs.seekg(chunkSize, std::ios::cur);
		}

		// skip 1 pad byte if size is odd
		if (chunkSize % 2 != 0) {
			ifs.seekg(1, std::ios::cur);
		}
	}

	if (!foundFmt || !foundData) {
		std::cout << "Missing fmt or data chunk" << std::endl;
		return 0;
	}

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
	return 0;
}

