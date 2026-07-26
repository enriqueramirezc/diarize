#include "ReadData.hpp"

// fetch data after file header
int ReadData::fetchData(FileHeader& h, std::ifstream& ifs) {
	samples.resize(h.SubChunk2Size / sizeof(int16_t));
	ifs.read(reinterpret_cast<char*>(samples.data()), h.SubChunk2Size);
	return 1;
}
