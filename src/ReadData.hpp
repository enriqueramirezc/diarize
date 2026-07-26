#ifndef READDATA_HPP
#define READDATA_HPP

#include "FileHeader.hpp"
#include <fstream>
#include <vector>

class ReadData {

 public:
    std::vector<int16_t> samples;
    int fetchData(FileHeader& h, std::ifstream& ifs);
};
#endif
