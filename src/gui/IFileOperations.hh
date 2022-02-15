
#pragma once

#include <filesystem>

class IFileOperations {
public:
  virtual ~IFileOperations() = default;
  virtual bool openFile(std::string const &, std::filesystem::path &) = 0;
  
};