#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>


// `.wav` file header
struct header {

	// RIFF chunk descriptor
	char ChunkID[4];
	uint32_t ChunkSize;
	char Format[4];

	// fmt sub-chunk
	char SubChunk1ID[4];
	uint32_t SubChunk1Size;
	uint16_t AudioFormat;
	uint16_t NumChannels;
	uint32_t SampleRate;
	uint32_t ByteRate;
	uint16_t BlockAlign;
	uint16_t BitsPerSample;

	// data sub-chunk
	uint32_t SubChunk2Size;
};

int main() {

	// Read file
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

	header h;

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
			h.SubChunk2Size = chunkSize;
			foundData = true;
		} else {
			// skip other chunks...
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

	std::cout << "Sample rate: " << h.SampleRate << std::endl;
	std::cout << "Channels: " << h.NumChannels << std::endl;
	std::cout << "Bits per sample: " << h.BitsPerSample << std::endl;
	std::cout << "Data size: " << h.SubChunk2Size << std::endl;

	return 0;
}

