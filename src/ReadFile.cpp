#include "ReadFile.hpp"

// fetch metatada from file header
int ReadFile::fetchMetadata(Wave& h, std::ifstream& ifs) {

	// RIFF chunk descriptor (always first 12 bytes)
	ifs.read(h.descriptor.ChunkID, 4);
	ifs.read(reinterpret_cast<char*>(&h.descriptor.ChunkSize), 4);
	ifs.read(h.descriptor.Format, 4);

	// check it's a valid file
	if (std::string(h.descriptor.ChunkID, 4) != "RIFF"
		|| std::string(h.descriptor.Format, 4) != "WAVE") {
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
			std::memcpy(h.format.SubChunk1ID, chunkID, 4);
			h.format.SubChunk1Size = chunkSize;
			ifs.read(reinterpret_cast<char*>(&h.format.AudioFormat), 2);
			ifs.read(reinterpret_cast<char*>(&h.format.NumChannels), 2);
			ifs.read(reinterpret_cast<char*>(&h.format.SampleRate), 4);
			ifs.read(reinterpret_cast<char*>(&h.format.ByteRate), 4);
			ifs.read(reinterpret_cast<char*>(&h.format.BlockAlign), 2);
			ifs.read(reinterpret_cast<char*>(&h.format.BitsPerSample), 2);
			// fmt chunk may be larger than 16. Skip extra bytes
			if (chunkSize > 16) {
				ifs.seekg(chunkSize - 16, std::ios::cur);
			}
			foundFmt = true;

		// found data header
		} else if (std::string(chunkID, 4) == "data") {
			std::memcpy(h.data.SubChunk2ID, chunkID, 4);
			h.data.SubChunk2Size = chunkSize;
			foundData = true;
			continue; // leave stream positioned at start of sample data
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

// fetch data after file header
int ReadFile::fetchData(Wave& d,std::ifstream& ifs) {
	d.data.samples.resize(d.data.SubChunk2Size / sizeof(int16_t));
	ifs.read(reinterpret_cast<char*>(d.data.samples.data()), d.data.SubChunk2Size);
	return ifs.gcount() == static_cast<std::streamsize>(d.data.SubChunk2Size) ? 1 : 0;
}