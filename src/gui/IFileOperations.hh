
#pragma once

#include <filesystem>

class IFileOperations {
public:
  virtual ~IFileOperations() = default;
  virtual bool openFile(std::filesystem::path &, std::string const &) = 0;
  
};