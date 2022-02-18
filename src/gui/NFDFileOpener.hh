
#pragma once

#include "IFileOperations.hh"

class NFDFileOpener : public IFileOperations {
public:
  NFDFileOpener() = default;
  bool openFileDialog(std::filesystem::path &, std::string_view const) override;
  bool saveFileDialog(std::filesystem::path &, std::string_view const) override;
};