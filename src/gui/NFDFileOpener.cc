
#include "NFDFileOpener.hh"
#include "nfd.h"


bool NFDFileOpener::openFile(std::filesystem::path &out, std::string const &extensionList) {
  nfdchar_t *path = NULL;

  nfdresult_t result = NFD_OpenDialog(extensionList.c_str(), nullptr, &path);
  if (result == NFD_OKAY) {
    out = path;
    NFD_Path_Free(path);
    return true;
  } else if (result == NFD_CANCEL) {
    NFD_Path_Free(path);
  }
  return false;
}