#include "ReadHeader.hpp"

// fetch metatada from file header
int ReadHeader::fetchMetadata(FileHeader& h, std::ifstream& ifs) {

	// RIFF chunk descriptor (always first 12 bytes)
	ifs.read(h.ChunkID, 4);
	ifs.read(reinterpret_cast<char*>(&h.ChunkSize), 4);
	ifs.read(h.Format, 4);

	// check it's a valid file
	if (std::string(h.ChunkID, 4) != "RIFF"
		|| std::string(h.Format, 4) != "WAVE") {
		std::cout << "Not a valid WAV file"
			<< std::endl;
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
            // http://unusedino.de/ec64/technical/formats/wav.html
			ifs.seekg(chunkSize, std::ios::cur);
		}

		// skip 1 pad byte if size is odd
		if (chunkSize % 2 != 0) {
			ifs.seekg(1, std::ios::cur);
		}
	}

	if (!foundFmt || !foundData) {
		std::cout << "Missing fmt or data chunk"
			<< std::endl;
		return 0;
	}
	return 1;
}