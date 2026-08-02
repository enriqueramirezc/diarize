#ifndef ReadHeader_HPP
#define ReadHeader_HPP
#include <fstream>
#include <iostream>
#include <cstring>
#include "Wave.hpp"

class ReadFile {
 public:
  int fetchMetadata(Wave& h, std::ifstream& ifs);
  int fetchData(Wave& d,std::ifstream& ifs);
};
#endif // ReadHeader_HPP