#include "c_ls.h"
#include <iostream>

namespace fs = std::filesystem;
using namespace std;

namespace c_ls_tools
{
    bool is_executable(const fs::directory_entry& entry)
    {
        struct stat st;
        if (lstat(entry.path().c_str(), &st) == 0)
        {
            return (st.st_mode & S_IXUSR) != 0;
        }

        return false;
    }

    bool endsWithExtension(const fs::directory_entry &entry, const string &extension)
    {
        std::string entryPath = entry.path().string();
        size_t dotPosition = entryPath.rfind('.');
        
        // Compare the entry's extension with the desired extension (case-insensitive)
        if (dotPosition != std::string::npos)
        {
            std::string entryExtension = entryPath.substr(dotPosition);
            return entryExtension == extension;
        }

        // No extension found in the entry's path
        return false;
    }
    
    void c_ls(const string &full_PATH_to_dir, bool list_hidden = false)
    {
        /* cout << "Contents of directory ( " << full_PATH_to_dir << " ):\n"; */
        try
        {
            // Create a vector to store directory entries
            vector<fs::directory_entry> entries;
            for (const auto &entry : fs::directory_iterator(full_PATH_to_dir))
            {
                // If list_hidden is false, skip hidden files and directories
                if (!list_hidden && entry.path().filename().string().front() == '.')
                {
                    continue;
                }
            
                // Store the directory entries in the vector
                entries.push_back(entry);
            }

            // Sort the vector alphabetically by filename
            std::sort
            (
                entries.begin(),
                entries.end(),
                [](const fs::directory_entry& a, const fs::directory_entry& b)
                {
                    return a.path().filename() < b.path().filename();
                }
            );

            // Print the sorted contents with color coding for directories and executables
            for (const auto &entry : entries)
            {
                cout << (endsWithExtension(entry, ".msi")  ? "\033[32m" : "");  // Set text color to    green   for     .msi    files
                cout << (endsWithExtension(entry, ".exe")  ? "\033[32m" : "");  // Set text color to    green   for     .exe    files
                cout << (endsWithExtension(entry, ".sh")   ? "\033[32m" : "");  // Set text color to    green   for     .sh     files
                cout << (endsWithExtension(entry, ".gz")   ? "\033[31m" : "");  // Set text color to    red     for     .gz     files
                cout << (endsWithExtension(entry, ".xz")   ? "\033[31m" : "");  // Set text color to    red     for     .xz     files
                cout << (endsWithExtension(entry, ".h")    ? "\033[35m" : "");  // Set text color to    red     for     .h      files
                cout << (endsWithExtension(entry, ".conf") ? "\033[33m" : "");  // Set text color to    red     for     .conf   files
                cout << (endsWithExtension(entry, ".c")    ? "\033[92m" : "");  // Set text color to    red     for     .c      files
                cout << (endsWithExtension(entry, ".cpp")  ? "\033[92m" : "");  // Set text color to    red     for     .cpp    files
                cout << (is_executable(entry) ? "\033[32m" : "");                         // Set text color to    green   for     exec    files
                cout << (entry.is_directory() ? "\e[1m\e[34m" : "");                      // Set text color to    blue    for     dirs
                cout << entry.path().filename();
                cout << "\033[0m";                                                        // Reset text color to default
                cout << '\n';                                                             // Print a newline
            }
        }
        catch (const exception& e)
        {
            cerr << "Error: " << e.what() << endl;
        }
    }
}

void c_ls(const vector<string>& args)
{
    if (args[1] == "-h")
    {
        if (args.size() == 4)
        {
            std::string directoryName = args[2] + " " + args[3];
            c_ls_tools::c_ls(directoryName, true);
        }
        else if (args.size() == 3)
        {
            c_ls_tools::c_ls(args[2], true);
        }
        else if (args.size() == 2)
        {
            c_ls_tools::c_ls(fs::current_path().string(), true);
        }
        else
        {
            std::cout << "Usage: ls -h <destination>\n";
        }
    }
    else
    {
        if (args.size() == 3)
        {
            std::string directoryName = args[1] + " " + args[2];
            c_ls_tools::c_ls(directoryName, false);
        }
        else if (args.size() == 2)
        {
            std::string command = args[0] + " \"" + args[1] + "\"";
            c_ls_tools::c_ls(args[1], false);
        }
        else if (args.size() == 1)
        {
            c_ls_tools::c_ls(fs::current_path().string(), false);
        }
        else
        {
            std::cout << "Usage: ls <destination> or ls -h <destination> to show hidden files as well\n";
        }
    }
}
