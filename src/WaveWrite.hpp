#ifndef WAVEWRITE_HPP
#define WAVEWRITE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

class WaveWrite {
 public:
  int write(const std::string& output, const std::vector<int16_t>& samples,
            uint32_t sampleRate, uint16_t numChannels, uint16_t bitsPerSample);
};
#endif // WAVEWRITE_HPP
