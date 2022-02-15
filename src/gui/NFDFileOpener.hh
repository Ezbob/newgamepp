
#pragma once

#include "IFileOperations.hh"

class NFDFileOpener : public IFileOperations {
public:
  NFDFileOpener() = default;
  bool openFile(std::filesystem::path &, std::string const &) override;
};