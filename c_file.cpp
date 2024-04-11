#include "c_file.h"
#include "base_tools.h"

namespace fs = std::filesystem;

namespace tools {
    std::uintmax_t getFileSize(const std::string& filePath) {
        try {
            return fs::file_size(filePath);
        } catch (const fs::filesystem_error& e) {
            error_message_no_halt("getFileSize", e.what());
            return 0; // Return 0 to indicate an error
        }
    }
    bool getFileMetadata(const std::string& filename, struct stat& fileStat) {
        if (stat(filename.c_str(), &fileStat) != 0) {
            return false;                                   // Error
        }
        return true;                                        // Success
    }
    void display_metadata(const std::string& full_PATH_to_file) {
        struct stat fileStat;
        if (!getFileMetadata(full_PATH_to_file, fileStat)) {
            error_message_no_halt("display_metadata", "Could not retrive metadata for file: '" + full_PATH_to_file + "'");
            return;
        }
        double MB = static_cast<double>(fileStat.st_size) / (1024 * 1024);
        std::cout << "File Size: " << fileStat.st_size << " bytes or " << MB << " megabytes\n";
        std::cout << "Last Access Time: " << std::ctime(&fileStat.st_atime);
        std::cout << "Last Modification Time: " << std::ctime(&fileStat.st_mtime);
        std::cout << "File Permissions: " << std::oct << fileStat.st_mode << std::dec << std::endl;
    }
}

void c_file(const std::vector<std::string>& args) {
    if (args::find_arg(args, "size")) {
        std::string filePath = args::processArgs(args, "size");
        std::uintmax_t file_size = tools::getFileSize(filePath);
        if (file_size == 0) {
            return;
        }
        double MB = static_cast<double>(file_size) / (1024 * 1024);
        std::cout << "file size: is '" << file_size << "' bytes = '" << MB << "' megabytes\n";
        return;
    }
    if (args::find_arg(args, "info")) {
        tools::display_metadata(args::processArgs(args, "info"));
        return;
    }
    std::cout << "usage: 'file' <'size', 'info'> <full_PATH_to_file>\n";
}
