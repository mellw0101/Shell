#include "c_ls.h"
#include <iostream>

namespace fs = std::filesystem;
namespace c_ls_tools {
    bool is_executable(const fs::directory_entry& entry) {
        struct stat st;
        if (lstat(entry.path().c_str(), &st) == 0) {
            return (st.st_mode & S_IXUSR) != 0;
        }
        return false;
    }
    bool endsWithExtension(const fs::directory_entry& entry, const std::string& extension) {
        std::string entryPath = entry.path().string();
        size_t dotPosition = entryPath.rfind('.');
        if (dotPosition != std::string::npos) {             // Compare the entry's extension with the desired extension (case-insensitive)
            std::string entryExtension = entryPath.substr(dotPosition);
            return entryExtension == extension;
        }
        return false;                                       // No extension found in the entry's path
    }
    void c_ls(const std::string& full_PATH_to_dir, bool list_hidden = false) {
        std::cout << "Contents of directory ( " << full_PATH_to_dir << " ):\n";
        try {
            std::vector<fs::directory_entry> entries;       // Create a vector to store directory entries
            for (const auto& entry : fs::directory_iterator(full_PATH_to_dir)) {
                if (!list_hidden && entry.path().filename().string().front() == '.') {  // If list_hidden is false, skip hidden files and directories
                    continue;
                }
                entries.push_back(entry);                   // Store the directory entries in the vector
            }
            std::sort(entries.begin(), entries.end(), [](const fs::directory_entry& a, const fs::directory_entry& b) {  // Sort the vector alphabetically by filename
                return a.path().filename() < b.path().filename();
            });
            for (const auto& entry : entries) {             // Print the sorted contents with color coding for directories and executables
                std::cout << (endsWithExtension(entry, ".msi")  ? "\033[32m" : "");  // Set text color to    green   for     .msi    files
                std::cout << (endsWithExtension(entry, ".exe")  ? "\033[32m" : "");  // Set text color to    green   for     .exe    files
                std::cout << (endsWithExtension(entry, ".sh")   ? "\033[32m" : "");  // Set text color to    green   for     .sh     files
                std::cout << (endsWithExtension(entry, ".gz")   ? "\033[31m" : "");  // Set text color to    red     for     .gz     files
                std::cout << (endsWithExtension(entry, ".xz")   ? "\033[31m" : "");  // Set text color to    red     for     .xz     files
                std::cout << (endsWithExtension(entry, ".h")    ? "\033[35m" : "");  // Set text color to    red     for     .h      files
                std::cout << (endsWithExtension(entry, ".conf") ? "\033[33m" : "");  // Set text color to    red     for     .conf   files
                std::cout << (endsWithExtension(entry, ".c")    ? "\033[92m" : "");  // Set text color to    red     for     .c      files
                std::cout << (endsWithExtension(entry, ".cpp")  ? "\033[92m" : "");  // Set text color to    red     for     .cpp    files
                std::cout << (is_executable(entry) ? "\033[32m" : "");               // Set text color to    green   for     exec    files
                std::cout << (entry.is_directory() ? "\e[1m\e[34m" : "");            // Set text color to    blue    for     dirs
                std::cout << entry.path().filename();
                std::cout << "\033[0m"; // Reset text color to default
                std::cout << std::endl; // Print a newline
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void c_ls(const std::vector<std::string>& args) {
    if (args[1] == "-h") {
        if (args.size() == 4) {
            std::string directoryName = args[2] + " " + args[3];
            c_ls_tools::c_ls(directoryName, true);
        } else if (args.size() == 3) {
            c_ls_tools::c_ls(args[2], true);
        } else if (args.size() == 2) {
            c_ls_tools::c_ls(fs::current_path().string(), true);
        } else {
            std::cout << "Usage: ls -h <destination>\n";
        }
    } else {
        if (args.size() == 3) {
            std::string directoryName = args[1] + " " + args[2];
            c_ls_tools::c_ls(directoryName, false);
        } else if (args.size() == 2) {
            std::string command = args[0] + " \"" + args[1] + "\"";
            c_ls_tools::c_ls(args[1], false);
        } else if (args.size() == 1) {
            c_ls_tools::c_ls(fs::current_path().string(), false);
        } else {
            std::cout << "Usage: ls <destination> or ls -h <destination> to show hidden files as well\n";
        }
    }
}
