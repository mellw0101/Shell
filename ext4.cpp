#include <fstream>

bool formatExt4(const std::string& devicePath) {
    std::fstream deviceStream;
    deviceStream.open(devicePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!deviceStream.is_open()) {
        return false; // Couldn't open the device
    }

    // Write superblock
    // Write block group descriptors
    // Initialize inode tables
    // Allocate data blocks
    // Create root directory
    // Setup journaling

    deviceStream.close();
    return true;
}
