#include "Wave.hpp"

// Print file metadata
void Wave::printHeaderInfo() {
	std::cout << "RIFF chunk ID: " << std::string(descriptor.ChunkID, 4) << std::endl;
	std::cout << "RIFF chunk size: " << descriptor.ChunkSize << std::endl;
	std::cout << "RIFF Format: " << std::string(descriptor.Format, 4) << std::endl;

	std::cout << "---------------"<< std::endl;

	std::cout << "fmt id: " << std::string(format.SubChunk1ID, 4) << std::endl;
	std::cout << "fmt size: " << format.SubChunk1Size << std::endl;
	std::cout << "fmt audio format: " << format.AudioFormat << std::endl;
	std::cout << "fmt Channels: " << format.NumChannels << std::endl;
	std::cout << "fmt Sample rate: " << format.SampleRate << std::endl;
	std::cout << "fmt byte range: " << format.ByteRate << std::endl;
	std::cout << "fmt Block align: " << format.BlockAlign << std::endl;
	std::cout << "fmt Bits per sample: " << format.BitsPerSample << std::endl;

	std::cout << "---------------"  << std::endl;

	std::cout << "Data id: " << std::string(data.SubChunk2ID, 4) << std::endl;
	// 29548544= ChunkSize - 44 (header) - 26 (skipped LIST chunk)
	std::cout << "Data size: " << data.SubChunk2Size << std::endl;
}

