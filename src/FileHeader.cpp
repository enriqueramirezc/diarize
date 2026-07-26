#include "FileHeader.hpp"

// Print file metadata
void FileHeader::printHeaderInfo() {
	std::cout << "RIFF chunk ID: " << std::string(ChunkID, 4) << std::endl;
	std::cout << "RIFF chunk size: " << ChunkSize << std::endl;
	std::cout << "RIFF Format: " << std::string(Format, 4) << std::endl;


	std::cout << "---------------"<< std::endl;

	std::cout << "fmt id: " << std::string(SubChunk1ID, 4) << std::endl;
	std::cout << "fmt size: " << SubChunk1Size << std::endl;
	std::cout << "fmt audio format: " << AudioFormat << std::endl;
	std::cout << "fmt Channels: " << NumChannels << std::endl;
	std::cout << "fmt Sample rate: " << SampleRate << std::endl;
	std::cout << "fmt byte range: " << ByteRate << std::endl;
	std::cout << "fmt Block align: " << BlockAlign << std::endl;
	std::cout << "fmt Bits per sample: " << BitsPerSample << std::endl;


	std::cout << "---------------"  << std::endl;

	std::cout << "Data id: " << std::string(SubChunk2ID, 4) << std::endl;
	// 29548544= ChunkSize - 44 (header) - 26 (skipped LIST chunk)
	std::cout << "Data size: " << SubChunk2Size << std::endl;
}