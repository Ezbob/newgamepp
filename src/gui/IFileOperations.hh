
#pragma once

#include <filesystem>
#include <string_view>

class IFileOperations {
public:
  virtual ~IFileOperations() = default;
  virtual bool openFile(std::filesystem::path &, std::string_view const) = 0;
  virtual bool saveFile(std::filesystem::path &, std::string_view const) = 0;
};