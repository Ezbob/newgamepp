
#pragma once

#include "IFileOpener.hh"

class NFDFileOpener : public IFileOpener {
public:
  NFDFileOpener() = default;
  bool openFile(std::string const &extensionList, std::filesystem::path &) override;
};