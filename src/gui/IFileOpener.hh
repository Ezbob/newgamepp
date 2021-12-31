
#pragma once

#include <filesystem>

class IFileOpener {
public:
  virtual ~IFileOpener() = default;
  virtual bool openFile(std::string const &extensionList, std::filesystem::path &) = 0;
};