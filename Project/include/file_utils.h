#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <filesystem>
#include <string>
#include <vector>

namespace files {

bool IsFile(const std::string& path);

bool IsDirectory(const std::string& path);

void ListFiles(const std::string& dir,
               std::vector<std::string>& result);

} // namespace files

#endif // FILE_UTILS_H
