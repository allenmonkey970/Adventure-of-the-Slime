#include "utils.h"
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <pwd.h>
#endif


// add mac os option
std::string GetLocalPath() {
#ifdef _WIN32
    char localPath[MAX_PATH];
    if (SHGetFolderPathA(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, localPath) == S_OK) {
        return {localPath};
    }
    throw std::runtime_error("Failed to get Local AppData path.");
#else
    const char* homedir;
    if ((homedir = getenv("HOME")) == nullptr) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    std::string localShare(homedir);
    localShare.append("/.local/share");
    return localShare;
#endif
}
