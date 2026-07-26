#ifndef READFILE_HPP
#define READFILE_HPP
#include <fstream>
#include <iostream>
#include <cstring>
#include "FileHeader.hpp"

class ReadFile {
 public:
  int fetchMetadata(FileHeader& h, std::ifstream& ifs);
 
};
#endif // READFILE_HPP