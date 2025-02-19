#include "utils.h"
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#elif defined(__APPLE__)
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#else
#include <unistd.h>
#include <pwd.h>
#endif

std::string GetLocalPath() {
#ifdef _WIN32
    char localPath[MAX_PATH];
    if (SHGetFolderPathA(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, localPath) == S_OK) {
        return {localPath};
    }
    throw std::runtime_error("Failed to get Local AppData path.");
#elif defined(__APPLE__)
    const char* homedir = getenv("HOME");
    if (!homedir) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    std::string localShare(homedir);
    localShare.append("/Library/Application Support");
    return localShare;
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
