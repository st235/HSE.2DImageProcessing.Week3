#include "file_utils.h"

namespace files {

namespace fs = std::filesystem;

bool IsFile(const std::string& path) {
    const fs::path filepath(path);
    std::error_code error_code;
    if (fs::is_regular_file(filepath, error_code)) {
        return true;
    }
    return false;
}

bool IsDirectory(const std::string& path) {
    const fs::path filepath(path);
    std::error_code error_code;
    if (fs::is_directory(filepath, error_code)) {
        return true;
    }
    return false;
}

void ListFiles(const std::string& dir, std::vector<std::string>& result) {
    for (const auto& entry: fs::directory_iterator(dir)) {
        const auto& path = entry.path();
        std::string raw_path(path.c_str());

        if (IsFile(raw_path)) {
            result.push_back(raw_path);
        } else if (IsDirectory(raw_path)) {
            ListFiles(raw_path, result);
        }
    }
}

}
