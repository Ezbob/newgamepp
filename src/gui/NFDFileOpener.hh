
#pragma once

#include "IFileOperations.hh"

class NFDFileOpener : public IFileOperations {
public:
  NFDFileOpener() = default;
  bool openFile(std::string const &extensionList, std::filesystem::path &) override;
};