#include "c_cp.h"
#include "base_tools.h"

namespace fs = std::filesystem;

bool calcCRC32(const std::string& filename, uint32_t& checksum) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        error_message_no_halt("calcCRC32", "Cannot open FILE: '" + filename + "'");
        return false;
    }
    uint32_t crc = 0;
    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        crc = static_cast<uint32_t>(crc32(crc, reinterpret_cast<const Bytef*>(buffer), static_cast<uInt>(file.gcount())));
    }
    file.close();
    checksum = crc;
    return true;
}
bool Checksum(const std::string& sourcePath, const std::string& destinationPath) {
    uint32_t source_checksum;
    uint32_t destination_checksum;
    if (!calcCRC32(sourcePath, source_checksum)) {
        error_message_no_halt("calcCRC32", "Failed to calculate checksum for FILE: '" + destinationPath + "'");
        return false;
    }
    if (!calcCRC32(destinationPath, destination_checksum)) {
        error_message_no_halt("calcCRC32", "Failed to calculate checksum for FILE: '" + destinationPath + "'");
        return false;
    }
    if (source_checksum != destination_checksum) {
        error_message_no_halt("Checksum", "Dosent Match!!");
        return false;
    }
    return true;
}
void copyFile(const std::string& sourcePath, const std::string& destinationPath) {
    std::ifstream source(sourcePath, std::ios::binary);     // open source file
    std::ofstream destination(destinationPath, std::ios::binary); // open destination file
    if (!source) {                                          // if source file coult not be opened tell user and exit function
        error_message_no_halt("copyFile", "Error opening source file: '" + sourcePath + "'" );
        return;
    }
    if (!destination) {                                     // if destination file could not be opened close function
        error_message_no_halt("copyFile", "Error opening destination file: '" + destinationPath + "'");
        return;
    }
    destination << source.rdbuf();                          // Use istreambuf_iterator to copy file
    source.close();                                         // closes source file
    destination.close();                                    // closes destination file
    Checksum(sourcePath, destinationPath);
}
void c_cp(const std::string& sourcePath, const std::string& destinationPath, bool force) {
    if (!fs::exists(sourcePath)) {
        error_message_no_halt("c_cp", "Source FILE: '" + sourcePath + "' does not exist");
        return;
    }
    if (!fs::exists(destinationPath)) {
        copyFile(sourcePath, destinationPath);
        return;
    }
    if (!force){
        char answer;
        std::cout << "c_cp: Destination FILE: '" << destinationPath << "' Does exist\nc_cp: do you want to overwrite FILE ?: ";
        std::cin >> answer;
        if (answer == 'Y' || answer == 'y') {
            copyFile(sourcePath, destinationPath);
        }
        return;
    }
    copyFile(sourcePath, destinationPath);
}

void cp(const std::vector<std::string>& args) {
    if (args.size() == 4) {
        if (args[3] == "-f") {
            c_cp(args[1], args[2], true);
            return;
        }
    }
    if (args.size() == 3) {
        c_cp(args[1], args[2], false);
        return;
    } else {
        std::cout << "Usage: cp <source> <destination> <-f> To force overwrite" << std::endl;
    }
}
