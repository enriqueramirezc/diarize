#ifndef CSVWRITE_HPP
#define CSVWRITE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

class CSVWrite {
 public:
  int write(const std::string& output, const std::vector<int16_t>& samples,
            uint32_t sampleRate, int numChannels);
};
#endif // CSVWRITE_HPP
