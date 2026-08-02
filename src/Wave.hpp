#ifndef WAVE_HPP
#define WAVE_HPP
#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>

// RIFF chunk descriptor
struct RIFFChunk {
	char ChunkID[4];
	uint32_t ChunkSize;
	char Format[4];
};

// fmt sub-chunk
struct FormatChunk {
	char SubChunk1ID[4];
	uint32_t SubChunk1Size;
	uint16_t AudioFormat;
	uint16_t NumChannels;
	uint32_t SampleRate;
	uint32_t ByteRate;
	uint16_t BlockAlign;
	uint16_t BitsPerSample;
};

// data sub-chunk
struct DataChunk {
  char SubChunk2ID[4];
	uint32_t SubChunk2Size;
  std::vector<int16_t> samples;
};

// `.wav` file header
class Wave {

 public:
  RIFFChunk descriptor;
  FormatChunk format;
  DataChunk data;

	void printHeaderInfo();

};
#endif // WAVE_HPP
