
#include "NFDFileOpener.hh"
#include "nfd.h"
#include <string>


bool NFDFileOpener::openFile(std::filesystem::path &out, std::string_view const extensionList) {
  nfdchar_t *path = nullptr;
  std::string extensions{extensionList};

  nfdresult_t result = NFD_OpenDialog(extensions.c_str(), nullptr, &path);
  if (result == NFD_OKAY) {
    out = path;
    NFD_Path_Free(path);
    return true;
  } else if (result == NFD_CANCEL) {
    NFD_Path_Free(path);
  }
  return false;
}


bool NFDFileOpener::saveFile(std::filesystem::path &out, std::string_view const extensionList) {
  nfdchar_t *path = nullptr;
  std::string extensions{extensionList};

  nfdresult_t result = NFD_SaveDialog(extensions.c_str(), nullptr, &path);
  if (result == NFD_OKAY) {
    out = path;
    NFD_Path_Free(path);
    return true;
  } else if (result == NFD_CANCEL) {
    NFD_Path_Free(path);
  }
  return false;
}
