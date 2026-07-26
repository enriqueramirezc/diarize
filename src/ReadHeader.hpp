#ifndef ReadHeader_HPP
#define ReadHeader_HPP
#include <fstream>
#include <iostream>
#include <cstring>
#include "FileHeader.hpp"

class ReadHeader {
 public:
  int fetchMetadata(FileHeader& h, std::ifstream& ifs);
 
};
#endif // ReadHeader_HPP