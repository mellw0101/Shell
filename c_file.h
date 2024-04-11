#ifndef C_FILE_H
#define C_FILE_H

#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include <ctime>
#include <chrono> // Include the chrono library for time formatting

namespace tools {
    std::uintmax_t getFileSize(const std::string& filePath);
    bool getFileMetadata(const std::string& filename, struct stat& fileStat);
    void display_metadata(const std::string& full_PATH_to_file);
}

void c_file(const std::vector<std::string>& args);

#endif // C_FILE_H
