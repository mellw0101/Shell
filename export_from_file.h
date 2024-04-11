#ifndef EXPORT_FROM_FILE_H
#define EXPORT_FROM_FILE_H

#include <sstream>
#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>

namespace tools {
    bool export_from_file(const std::string& FILEname, std::string& evar);
}

void eff(const std::vector<std::string>& args);
#endif // EXPORT_FROM_FILE_H
