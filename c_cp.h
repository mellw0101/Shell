#ifndef C_CP_H
#define C_CP_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <filesystem>
#include <vector>
#include <zlib.h>

bool calcCRC32(const std::string& filename, uint32_t& checksum);
bool Checksum(const std::string& sourcePath, const std::string& destinationPath);
void copyFile(const std::string& sourcePath, const std::string& destinationPath);
void c_cp(const std::string& sourcePath, const std::string& destinationPath, bool force = false);
void cp(const std::vector<std::string>& args);

#endif // C_CP_H
