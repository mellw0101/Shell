#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

std::vector<std::string> get_block_devices() {
    std::vector<std::string> devices;
    for (const auto& entry : std::filesystem::directory_iterator("/sys/class/block")) {
        devices.push_back(entry.path().filename());
    }
    return devices;
}

std::string read_sysfs_attribute(const std::string& path) {
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        return line;
    }
    return "";
}

void print_block_device_info(const std::string& device) {
    std::string path = "/sys/class/block/" + device;
    std::string dev_size = read_sysfs_attribute(path + "/size");
    std::string dev_type = read_sysfs_attribute(path + "/device/type");
    std::cout << "Device: " << device
              << " | Size (in sectors): " << dev_size
              << " | Type: " << dev_type << std::endl;
}

int main() {
    std::vector<std::string> devices = get_block_devices();
    for (const auto& device : devices) {
        print_block_device_info(device);
    }
    return 0;
}
