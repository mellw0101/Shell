#include <algorithm>                                        // for sorting
#include <array>
#include <atomic>
#include <cerrno>
#include <chrono>
#include <complex>
#include <condition_variable>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/misc.h>
#include <cryptopp/sha.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <curses.h>
#include <errno.h>
#include <ext2fs/ext2fs.h>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <iostream>                                         // Include the header for std::cout
#include <iterator>
#include <mutex>
#include <ncurses.h>
#include <numeric>
#include <openssl/sha.h>
#include <ostream>
#include <pthread.h>
#include <queue>
#include <readline/history.h>
#include <readline/readline.h>
#include <regex>
#include <sched.h>
#include <set>
#include <spawn.h>
#include <sstream>
#include <sstream>                                          // Include the header for std::ostringstream
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/mount.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <zlib.h>

#include "c_cp.h"
#include "base_tools.h"
#include "math.h"
#include "pipe.h"
#include "c_ls.h"
#include "export_from_file.h"
#include "string_manipulation.h"

/*
const size_t BLOCK_SIZE = 512;

void writeTarHeader(std::ofstream &outStream, const std::string& fileName, size_t fileSize) {
    char header[BLOCK_SIZE] = {0};
    strncpy(header, fileName.c_str(), 99);
    snprintf(header + 100, 8, "%07o", 0644);
    snprintf(header + 108, 8, "%07o", 0);
    snprintf(header + 116, 8, "%07o", 0);
    snprintf(header + 124, 12, "%011lo", static_cast<unsigned long>(fileSize));
    snprintf(header + 136, 12, "%011lo", static_cast<unsigned long>(std::time(nullptr)));
    header[156] = '0';

    // Prepare checksum field and calculate checksum
    memset(header + 148, ' ', 8);
    unsigned int checksum = 0;
    for (size_t i = 0; i < BLOCK_SIZE; ++i) {
        checksum += static_cast<unsigned char>(header[i]);
    }
    snprintf(header + 148, 8, " %06o", checksum);

    outStream.write(header, BLOCK_SIZE);
}

void writeTarPadding(std::ofstream &outStream, size_t fileSize) {
    size_t paddingSize = (BLOCK_SIZE - (fileSize % BLOCK_SIZE)) % BLOCK_SIZE;
    std::vector<char> padding(paddingSize, '\0');
    outStream.write(padding.data(), paddingSize);
}

void writeFileToTar(std::ofstream &outFile, const std::string &filePath, const std::string &fileName) {
    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Cannot open file: " << filePath << std::endl;
        return;
    }

    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    writeTarHeader(outFile, fileName, fileSize);

    std::vector<char> buffer(fileSize);
    inFile.read(buffer.data(), fileSize);
    outFile.write(buffer.data(), fileSize);

    writeTarPadding(outFile, fileSize);
}

bool compressFolder(const std::string& folderPath, const std::string& tarPath) {
    std::ofstream outFile(tarPath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Cannot open output tar file.\n";
        return false;
    }

    DIR* dir = opendir(folderPath.c_str());
    if (dir == nullptr) {
        std::cerr << "Cannot open directory.\n";
        return false;
    }

    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type != DT_REG) {
            continue;
        }

        std::string filePath = folderPath + "/" + entry->d_name;
        writeFileToTar(outFile, filePath, entry->d_name);
    }

    closedir(dir);
    outFile.close();
    return true;
}


void gz(const std::vector<std::string>& args) {
    if (args::find_arg(args, "-sF")) {
        if (compressFolder(args::processArgs(args, "-sF"), args::processArgs(args, "-o"))) {
            std::cout << "SUCCESSFULL\n";
        } else {
            std::cout << "Failed to compress\n";
        }
        return;
    }
}*/


// std::future<void> runBinaryAsync_with_env_wargs(const std::string& binaryPath, const std::vector<std::string>& args) {
//     return std::async(std::launch::async, [&binaryPath, &args] {
//         std::string LD_PRELOAD = "LD_PRELOAD=/usr/local/lib/libgamemodeauto.so";
//         std::string XAUTHORITY = "XAUTHORITY=" + get_vars::get_XAUTHORITY_var();
//         std::string PATH = "PATH=" + get_vars::get_PATH_var();
//         std::string DISPLAY = "DISPLAY=" + get_vars::get_DISPLAY_var();
//         std::string TERM = "TERM=" + get_vars::get_TERM_var();
//         std::string COLORTERM = "COLORTERM=" + get_vars::get_COLORTERM_var();
//         std::string HOME = "HOME=" + get_vars::get_HOME_var();
//         std::string USER = "USER=" + get_vars::get_USER::get_USER_var();
//         std::string DBUS_SESSION_BUS_ADDRESS = "DBUS_SESSION_BUS_ADDRESS=" + get_vars::get_DBUS_SESSION_BUS_ADDRESS_var();
//         std::string DPCPP_HOME = "DPCPP_HOME=" + get_vars::get_DPCPP_HOME_var();
//         std::string XDG_CURRENT_DESKTOP = "XDG_CURRENT_DESKTOP=" + get_vars::get_XDG_CURRENT_DESKTOP_var();
//         std::string XDG_SESSION_CLASS = "XDG_SESSION_CLASS=" + get_vars::get_XDG_SESSION_CLASS_var();
//         std::string XDG_CONFIG_DIRS = "XDG_CONFIG_DIRS=" + get_vars::get_XDG_CONFIG_DIRS_var();
//         std::string XDG_SEAT = "XDG_SEAT=" + get_vars::get_XDG_SEAT_var();
//         std::string XDG_RUNTIME_DIR = "XDG_RUNTIME_DIR=" + get_vars::get_XDG_RUNTIME_DIR_var();
//
//
//
//
//         std::vector<char*> argv;
//         for (const std::string& arg : args) {       // Build the command string
//             argv.push_back(const_cast<char*>(arg.c_str()));      // Add each argument to the argv vector
//         }
//         argv.push_back(nullptr);  // The last element must be nullptr
//
//         pid_t pid;
//
//         // Get the pointer to the argv vector
//         char* const* argvPtr = argv.data();
//
//         char* const envp[] = {
//             //const_cast<char*>("XDG_SESSION_PATH=/org/freedesktop/DisplayManager/Session0"),
//             //const_cast<char*>("XCURSOR_SIZE=24"),
//             //const_cast<char*>("XDG_SESSION_DESKTOP=KDE"),
//             //const_cast<char*>("XDG_SESSION_TYPE=x11"),
//             //const_cast<char*>("XDG_SEAT_PATH=/org/freedesktop/DisplayManager/Seat0"),
//             // const_cast<char*>("XCURSOR_THEME=breeze_cursors"),
//             // const_cast<char*>(LD_PRELOAD.c_str()),
//             const_cast<char*>(DPCPP_HOME.c_str()),
//             const_cast<char*>(DBUS_SESSION_BUS_ADDRESS.c_str()),
//             const_cast<char*>(COLORTERM.c_str()),
//             const_cast<char*>(TERM.c_str()),
//             const_cast<char*>(XDG_CONFIG_DIRS.c_str()),
//             const_cast<char*>(XDG_RUNTIME_DIR.c_str()),
//             const_cast<char*>(XDG_SEAT.c_str()),
//             const_cast<char*>(XDG_SESSION_CLASS.c_str()),
//             const_cast<char*>(XDG_CURRENT_DESKTOP.c_str()),
//             const_cast<char*>(XAUTHORITY.c_str()),
//             const_cast<char*>(USER.c_str()),
//             const_cast<char*>(DISPLAY.c_str()),
//             const_cast<char*>(HOME.c_str()),
//             const_cast<char*>(PATH.c_str()),
//             nullptr
//         };
//
//         static posix_spawn_file_actions_t actions;
//         static posix_spawnattr_t attr;
//
//         posix_spawn_file_actions_init(&actions);
//         posix_spawnattr_init(&attr);
//
//
//         int status;
//         if (posix_spawn(&pid, binaryPath.c_str(), &actions, &attr, argvPtr, envp) == 0) {
//             // Wait for the child process to finish
//             waitpid(pid, &status, 0);
//         } else {
//             throw std::runtime_error("Error spawning the process: " + std::string(strerror(errno)));
//         }
//     });
// }

// if (args[0] == "!!") {
    //     if (historyIndex >= 0 && historyIndex < static_cast<int>(commandHistory.size())) {
    //         executeStoredCommand(commandHistory[historyIndex]);
    //     }
    // } else if (args[0][0] == '!' && args[0].size() > 1) {
    //     int commandNumber = std::stoi(args[0].substr(1));
    //     if (commandNumber >= 1 && commandNumber <= static_cast<int>(commandHistory.size())) {
    //         executeStoredCommand(commandHistory[commandNumber - 1]);
    //     }
    // } else if (args[0] == "cf") {                           // create_file function
    //     cf(args);
    // } else if (args[0] == "eff") {                          // export_from_file function
    //     eff(args);
    // } else if (args[0] == "itf") {                          // inputF function to append input to target
    //     itf(args);
    // } else if (args[0] == "run") {
    //     if (args.size() == 2) {
    //         try {
    //             auto asyncTask = mellwscommands::test::runBinaryAsync_with_env(args[1]);
    //             asyncTask.get(); // Wait for the async task to finish
    //         } catch (const std::exception& e) {
    //             std::cerr << "Error: " << e.what() << std::endl;
    //         }
    //     }
    // } else if (args[0] == "test") {
    //     std::cout << search_in_PATH(args[1]) << "\n";
    // } else if (args[0] == "og") {                           // run commands with execvp
    //     std::vector<std::string> newArgs(args.begin() + 1, args.end());
    //     std::vector<char*> argv;
    //         for (const std::string& arg : newArgs) {
    //             argv.push_back(const_cast<char*>(arg.c_str()));
    //         }
    //         argv.push_back(nullptr);  // Null-terminate the array
    //         pid_t pid = fork();                             // Fork a child process
    //         if (pid == -1) {                                // Forking failed
    //             perror("fork");
    //         } else if (pid == 0) {                          // This is the child process
    //             if (execvp(argv[0], argv.data()) == -1) {   // Execute the command
    //                 if (errno == ENOENT) {
    //                 std::cerr << "Shell: '" << args[0] << "' no such file or directory\n";
    //             } else {
    //                 perror("execvp");
    //             }
    //             exit(EXIT_FAILURE);
    //             }
    //         } else {                                        // This is the parent process
    //             int status;
    //             if (waitpid(pid, &status, 0) == -1) {       // Wait for the child to finish
    //                 perror("waitpid");
    //             }
    //         }
    // } else if (args[0] == "cd") {                           // cd to move change active directory
    //     cd(args);
    // } else if (args[0] == "cp") {                           // c_cp function
    //     if (args.size() == 4) {
    //         if (args[3] == "true") {
    //             c_cp(args[1], args[2], true);
    //             return;
    //         }
    //     }
    //     if (args.size() == 3) {
    //         c_cp(args[1], args[2], false);
    //     } else {
    //         std::cout << "Usage: cp <source> <destination> <true> To force overwrite" << std::endl;
    //     }
    // } else if (args[0] == "ls") {                           // c_ls function
    //     c_ls(args);
    // } else if (args[0] == "mkdir") {
    //     cmkdir(args);
    // } else if (args[0] == "math") {
    //     math(args);
    // } else if (tools::find_arg(args, "nano")) {
    //     return;
    // } else if (args[0] == "file") {
    //     c_file(args);
    // } else if (tools::find_arg(args, "cwd")) {
    //      std::cout << get_vars::get_CURRENT_WORKING_DIRECTORY_var() << "\n";
    // } else if (args[0] == "PATH") {
    //     std::cout << get_vars::get_PATH_var() << "\n";
    // } else if (args[0] == "wine") {
    //     wine(args);
    // } else if (args.back() == "&") {
    //     std::string command = search_in_PATH(args[0]);
    //     char** argv = mellwscommands::vectorToArgv(args);
    //     if (command == "Command not found in any PATH directory.") {
    //         return;
    //     }
    //     startjob(args[0].c_str() , argv);
    //     mellwscommands::cleanUpArgv(argv);
    // } else if (args[0] == "jobs") {
    //     listJobs();
    // } else if (args[0] == "killjob") {
    //     terminateJob(stringToInt(args[1]) - 1);
    // } else if (args[0] == "bf") {
    //     bringJobToForeground(stringToInt(args[1]) - 1);
    // } else if (args[0] == "lf") {
    //     listFiles(args[1]);
    // } else {
    //     std::string binaryPath = search_in_PATH(args[0]);
    //     // std::vector<std::string> newArgs(args.begin() + 1, args.end());
    //
    //     try {
    //         auto asyncTask = mellwscommands::test::runBinaryAsync_with_env_wargs(binaryPath, args);
    //         asyncTask.get(); // Wait for the async task to finish
    //     } catch (const std::exception& e) {
    //         std::cerr << "Error: " << e.what() << std::endl;
    //
    //
    //         std::vector<char*> argv;
    //         for (const std::string& arg : args) {
    //             argv.push_back(const_cast<char*>(arg.c_str()));
    //         }
    //         argv.push_back(nullptr);  // Null-terminate the array
    //
    //         // Fork a child process
    //         pid_t pid = fork();
    //
    //         if (pid == -1) {
    //             // Forking failed
    //             perror("fork");
    //         } else if (pid == 0) {
    //             // This is the child process
    //             // Execute the command
    //             if (execvp(argv[0], argv.data()) == -1) {
    //                 if (errno == ENOENT) {
    //                 std::cerr << "Shell: '" << args[0] << "' no such file or directory\n";
    //             } else {
    //                 perror("execvp");
    //             }
    //             exit(EXIT_FAILURE);
    //             }
    //         } else {
    //             // This is the parent process
    //             // Wait for the child to finish
    //             int status;
    //             if (waitpid(pid, &status, 0) == -1) {
    //                 perror("waitpid");
    //             }
    //         }
    //     }
    // return;
    // }

// void executeStoredCommand(const std::string& command) {
//     std::vector<std::string> storedArgs;
//     std::istringstream iss(command);
//     std::string token;
//     while (iss >> token) {
//         storedArgs.push_back(token);
//     }
//     executeCommand(storedArgs);
// }

namespace SourceCodeLinks {
    /*
    bash source code git clone git://git.savannah.gnu.org/bash.git
    konsole source code git clone https://invent.kde.org/utilities/konsole.git
    https://git.savannah.gnu.org/git/
    */
}

// void loadCommandHistory() {                                 // Function to load command history from a file
//     std::ifstream historyFile("/home/mellw/.ShellHistory");
//     if (historyFile.is_open()) {
//         std::string line;
//         while (std::getline(historyFile, line)) {
//             commandHistory.push_back(line);
//             add_history(line.c_str());
//         }
//         historyFile.close();
//     }
// }
// void saveCommandHistory() {                                 // Function to save command history to a file
//     std::ofstream historyFile("/home/mellw/.ShellHistory");
//     if (historyFile.is_open()) {
//         for (const std::string& command : commandHistory) {
//             historyFile << command << std::endl;
//         }
//         historyFile.close();
//     }
// }
// void addToHistory(const std::string& command) {             // Function to add a command to history
//     commandHistory.push_back(command);
//     historyIndex = commandHistory.size() - 1;
// }
// void addToHistory(const std::vector<std::string> &args) {
//     //std::string result = std::accumulate(args.begin(), args.end(), std::string(""));
//      std::string result = std::accumulate(args.begin(), args.end(), std::string(""),
//                                          [](const std::string& a, const std::string& b) {
//                                              return a + " " + b;
//                                          });
//
//
//
//     commandHistory.push_back(result);
//     historyIndex = commandHistory.size() - 1;
// }
// void displayHistory() {
//     std::cout << "Command History:" << std::endl;
//     for (const std::string& cmd : commandHistory) {
//         std::cout << cmd << "\n";
//     }
// }

namespace find_PATH {
    std::vector<std::string> splitPath(const std::string& pathVar) {
        std::vector<std::string> paths;
        std::istringstream iss(pathVar);
        std::string path;
        while (std::getline(iss, path, ':')) {
            paths.push_back(path);
        }
        return paths;
    }
    void removePrefixFromVector(std::vector<std::string>& vec, const std::string& prefix, const std::string& phrase) {
        size_t prefixLength = prefix.length();

        while (true) {
            bool found = false;

            for (auto it = vec.begin(); it != vec.end(); ++it) {
                if (it->find(prefix + phrase) == 0) {
                    // Found a match, remove the prefix + phrase
                    it->erase(0, prefixLength + phrase.length());
                    found = true;
                }
            }

            // If no more matches are found, exit the loop
            if (!found) {
                break;
            }
        }
    }
    bool get_first_Path(const std::string& phrase) {
        std::string pathEnv =  get_vars::get_PATH_var();

        std::vector<std::string> paths = splitString(pathEnv, ':');

        bool found = false;
        for (const std::string& path : paths) {
            // Check if the phrase exists in the current path
            std::string fullPath = path + "/" + phrase;
            if (access(fullPath.c_str(), F_OK) == 0) {
                std::cout << "Found in path: " << path << std::endl;
                // You can remove the path from the search here
                // Optionally, you can break the loop if you only want to find the first occurrence
                // break;
            }
        }

        if (!found) {
            std::cout << "Phrase not found in any PATH directory." << std::endl;
        }

        return found;
    }
    void get_current_Path(const std::string& phrase) {
        char* pathEnv = std::getenv("PATH");
        if (pathEnv == nullptr) {
            std::cerr << "PATH environment variable is not set." << std::endl;
            return;
        }

        std::vector<std::string> paths = splitPath(pathEnv);

        for (const std::string& path : paths) {
            // Check if the phrase exists in the current path
            std::string fullPath = path + "/" + phrase;
            if (access(fullPath.c_str(), F_OK) == 0) {
                std::cout <<  fullPath << std::endl;
            }
        }
    }
    void listOccurrencesInPath_old(const std::string& phrase) {
        char* pathEnv = std::getenv("PATH");
        if (pathEnv == nullptr) {
            std::cerr << "PATH environment variable is not set." << std::endl;
            return;
        }

        std::vector<std::string> paths = splitPath(pathEnv);
        std::set<std::string> foundPaths;

        for (const std::string& path : paths) {
            // Check if the phrase exists in the current path
            std::string fullPath = path + "/" + phrase;
            if (access(fullPath.c_str(), F_OK) == 0) {
                foundPaths.insert(path);
            }
        }

        if (foundPaths.empty()) {
            std::cout << "Phrase not found in any PATH directory." << std::endl;
        } else {
            std::cout << "Found in the following PATH directories:" << std::endl;
            for (const std::string& path : foundPaths) {
                std::cout << path << std::endl;
            }
        }
    }
    void listOccurrencesInPath(const std::string& phrase) {
        char* pathEnv = std::getenv("PATH");
        if (pathEnv == nullptr) {
            std::cerr << "PATH environment variable is not set." << std::endl;
            return;
        }

        std::vector<std::string> paths = splitPath(pathEnv);
        std::set<std::string> foundPaths;

        for (const std::string& path : paths) {
            // Check if the phrase exists in the current path
            std::string fullPath = path + "/" + phrase;
            if (access(fullPath.c_str(), F_OK) == 0) {
                foundPaths.insert(fullPath); // Store the full path, not just the directory
            }
        }

        if (foundPaths.empty()) {
            std::cout << "Phrase not found in any PATH directory." << std::endl;
        } else {
            std::cout << "Found in the following PATH directories:" << std::endl;
            for (const std::string& path : foundPaths) {
                std::cout << path << std::endl;
            }
        }
    }
    std::vector<std::string> findPaths(const std::string& phrase) {
        char* pathEnv = std::getenv("PATH");
        std::vector<std::string> foundPaths;

        if (pathEnv == nullptr) {
            std::cerr << "PATH environment variable is not set." << std::endl;
            return foundPaths;
        }

        std::vector<std::string> paths = splitPath(pathEnv);

        for (const std::string& path : paths) {
            // Check if the phrase exists in the current path
            std::string fullPath = path + "/" + phrase;
            if (access(fullPath.c_str(), F_OK) == 0) {
                foundPaths.push_back(fullPath);
            }
        }

        return foundPaths;
    }
    void list(const std::string& phraseToFind) {
        std::vector<std::string> foundPaths = findPaths(phraseToFind);

        if (foundPaths.empty()) {
            std::cout << "Phrase not found in any PATH directory." << std::endl;
        } else {
            std::cout << "Found in the following paths:" << std::endl;
            for (const std::string& path : foundPaths) {
                std::cout << path << std::endl;
            }
        }
        return;
    }
}
std::stringstream findCommand(const std::string& command) {
    std::array<char, 128> buffer;
    std::stringstream resultStream;

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error executing the command." << std::endl;
        return resultStream; // Return an empty stringstream
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        resultStream << buffer.data(); // Append to the stringstream
    }
    pclose(pipe);
    return resultStream;
}
namespace threds_tools {
    // simple function to get numCores
    int cpu_Cores(){
        unsigned int numCores = std::thread::hardware_concurrency();
        std::cout << "Number of CPU cores: " << numCores << std::endl;
        return numCores;
    }
    // Function to run Geekbench with a given program path
    int runGeekbench(const std::string& program) {
        char* const argv[] = {const_cast<char*>(program.c_str()), nullptr};
        char* const empty_env[] = {nullptr};

        pid_t pid;
        if (posix_spawn(&pid, program.c_str(), nullptr, nullptr, argv, empty_env) != 0) {
            return -1; // Error spawning process
        }

        int status;
        if (waitpid(pid, &status, 0) == -1) {
            return -2; // Error waiting for process
        }

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status); // Return the exit status
        } else {
            return -3; // Process did not exit normally
        }
    }
    // Function to run Geekbench in parallel with all available threads
    void runGeekbenchInParallel(const std::string& program) {
        // Get the number of hardware threads available
        unsigned int numThreads = std::thread::hardware_concurrency();
        std::cout << "Number of CPU cores: " << numThreads << std::endl;

        std::vector<std::thread> threads;

        // Create and start threads to run Geekbench
        for (unsigned int i = 0; i < numThreads; ++i) {
            threads.push_back(std::thread([program]() {
                int result = runGeekbench(program);
                if (result == 0) {
                    std::cout << "Thread completed successfully." << std::endl;
                } else {
                    std::cerr << "Thread failed with result: " << result << std::endl;
                }
            }));
        }

        // Wait for all threads to finish
        for (auto& thread : threads) {
            thread.join();
        }

        std::cout << "All threads have completed running Geekbench." << std::endl;
    }
}
namespace c_html {
    int main_html() {

        return 0;
    }
}
namespace unused_includes {
    // #include <algorithm>                                        // for sorting
    // #include <array>
    // #include <atomic>
    // #include <cerrno>
    // #include <chrono>
    // #include <complex>
    // #include <condition_variable>
    // #include <cryptopp/filters.h>
    // #include <cryptopp/hex.h>
    // #include <cryptopp/misc.h>
    // #include <cryptopp/sha.h>
    // #include <cstddef>
    // #include <cstdint>
    // #include <cstdio>
    // #include <cstdlib>
    // #include <cstring>
    // #include <ctime>
    // #include <curses.h>
    // #include <errno.h>
    // #include <ext2fs/ext2fs.h>
    // #include <fcntl.h>
    // #include <filesystem>
    // #include <fstream>
    // #include <functional>
    // #include <getopt.h>
    // #include <iomanip>
    // #include <iostream>
    // #include <iostream>                                         // Include the header for std::cout
    // #include <iterator>
    // #include <mutex>
    // #include <ncurses.h>
    // #include <openssl/sha.h>
    // #include <ostream>
    // #include <pthread.h>
    // #include <queue>
    // #include <regex>
    // #include <vector>
    // #include <zlib.h>
    // #include <sys/syscall.h>
    // #include <sys/types.h>
    // #include <sys/wait.h>
    // #include <thread>
    // #include <stdexcept>
    // #include <stdio.h>
    // #include <stdlib.h>
    // #include <string>
    // #include <sys/mount.h>

}

void runProgram(const std::string& program) {
    if (fork() == 0) {
        // Child process
        execlp(program.c_str(), program.c_str(), nullptr);
        perror("execlp failed"); // Print an error message if execlp fails
        exit(1);
    } else {
        // Parent process
        wait(NULL); // Wait for the child process to finish
    }
}
void runProgramWargs(const std::string &program, const std::vector<std::string> &args) {
    std::vector<char *> argv;
    for (const std::string &arg : args) {
        argv.push_back(const_cast<char *>(arg.c_str()));
    }
    argv.push_back(nullptr);

    if (fork() == 0) {
        // Child process
        execvp(program.c_str(), argv.data());
        perror("execvp failed"); // Print an error message if execvp fails
        exit(1);
    } else {
        // Parent process
        wait(NULL); // Wait for the child process to finish
    }
}
void runProgramWithInput(const std::string& program, const std::vector<std::string>& args, const std::string& input) {
    std::vector<char*> argv;
    for (const std::string& arg : args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        close(pipefd[1]);  // Close the write end of the pipe

        // Redirect stdin to read from the pipe
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2 failed");
            exit(1);
        }

        close(pipefd[0]);  // Close the read end of the pipe

        // Execute the program
        execvp(program.c_str(), argv.data());
        perror("execvp failed");
        exit(1);
    } else {
        // Parent process
        close(pipefd[0]);  // Close the read end of the pipe

        // Write the input to the child process's stdin through the pipe
        if (write(pipefd[1], input.c_str(), input.length()) == -1) {
            perror("write failed");
            exit(1);
        }

        close(pipefd[1]);  // Close the write end of the pipe

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int exitStatus = WEXITSTATUS(status);
            std::cout << "Child process exited with status: " << exitStatus << std::endl;
        }
    }
}
namespace test {
    std::future<void> runBinaryAsync_with_parent_env_wargs(const std::string& binaryPath, const std::vector<std::string>& args) {
        return std::async(std::launch::async, [&binaryPath, &args] {

            std::vector<char*> argv;
            for (const std::string& arg : args) {       // Build the command string
                argv.push_back(const_cast<char*>(arg.c_str()));      // Add each argument to the argv vector
            }
            argv.push_back(nullptr);  // The last element must be nullptr

            pid_t pid;

            // Get the pointer to the argv vector
            char* const* argvPtr = argv.data();

            char* const* envp = environ;

            static posix_spawn_file_actions_t actions;
            static posix_spawnattr_t attr;

            posix_spawn_file_actions_init(&actions);
            posix_spawnattr_init(&attr);


            int status;
            if (posix_spawn(&pid, binaryPath.c_str(), &actions, &attr, argvPtr, envp) == 0) {
                // Wait for the child process to finish
                waitpid(pid, &status, 0);
            } else {
                throw std::runtime_error("Error spawning the process: " + std::string(strerror(errno)));
            }
        });
    }
}
namespace run {
    char runExecutable_6(const std::string& program, const std::vector<std::string>& args) {
        // Initialize file actions
        posix_spawn_file_actions_t file_actions;
        if (posix_spawn_file_actions_init(&file_actions) != 0) {
            return -1; // Error initializing file actions
        }

        struct FileActionsRAII {
            posix_spawn_file_actions_t* fa;
            FileActionsRAII(posix_spawn_file_actions_t* fa) : fa(fa) {}
            ~FileActionsRAII() { posix_spawn_file_actions_destroy(fa); }
        } fileActionsRAII(&file_actions);

        // Create argv
        std::vector<const char*> argv(args.size() + 2, nullptr);
        argv[0] = program.c_str();
        for (size_t i = 0; i < args.size(); ++i) {
            argv[i + 1] = args[i].c_str();
        }

        pid_t pid;
        if (posix_spawn(&pid, program.c_str(), &file_actions, NULL, const_cast<char* const*>(argv.data()), environ) != 0) {
            return -2; // Error spawning process
        }

        int status;
        if (waitpid(pid, &status, 0) == -1) {
            return -3; // Error waiting for process
        }

        if (WIFEXITED(status)) {
            return static_cast<char>(WEXITSTATUS(status)); // Return the exit status as a char
        } else {
            return -4; // Process did not exit normally
        }
    }
    int program_start_wargs(const std::string& program, const std::vector<std::string>& args) {
        char result = runExecutable_6(program, args);
        if (result == -1) {
            std::cerr << "Error initializing file actions." << std::endl;
        } else if (result == -2) {
            std::cerr << "Error spawning process." << std::endl;
        } else if (result == -3) {
            std::cerr << "Error waiting for process." << std::endl;
        } else if (result == -4) {
            std::cerr << "Process did not exit normally." << std::endl;
        } else {
            std::cout << "Process exited with status: " << static_cast<int>(result) << std::endl;
        }
        return result;
    }
    char runExecutable(const std::string& program) {
        char* empty_env[] = {nullptr};
        pid_t pid;
        if (posix_spawn(&pid, program.c_str(), nullptr, nullptr, empty_env, environ) != 0) {
            return -1; // Error spawning process
        }

        int status;
        if (waitpid(pid, &status, 0) == -1) {
            return -2; // Error waiting for process
        }

        if (WIFEXITED(status)) {
            return static_cast<char>(WEXITSTATUS(status)); // Return the exit status as a char
        } else {
            return -3; // Process did not exit normally
        }
    }
    int program_start(const std::string& program) {

        char result = runExecutable(program);

        if (result == -1) {
            std::cerr << "Error spawning process." << std::endl;
        } else if (result == -2) {
            std::cerr << "Error waiting for process." << std::endl;
        } else if (result == -3) {
            std::cerr << "Process did not exit normally." << std::endl;
        } else {
            std::cout << "Process exited with status: " << static_cast<int>(result) << std::endl;
        }

        return result;
    }
    int runExecutable_7(const std::string& program, const std::vector<std::string>& args) {
        posix_spawn_file_actions_t file_actions;
        posix_spawn_file_actions_init(&file_actions);

        std::vector<const char*> argv;
        argv.reserve(args.size() + 2); // Reserve space to avoid reallocations
        argv.push_back(program.c_str());
        for (const auto& arg : args) {
            argv.push_back(arg.c_str());
        }
        argv.push_back(nullptr);

        pid_t pid;
        if (posix_spawn(&pid, program.c_str(), &file_actions, NULL, const_cast<char* const*>(argv.data()), environ) != 0) {
            perror("posix_spawn");
            return -1;
        }

        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return -2;
        }

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return -3;
        }
    }
    void fullRun(const std::string& program) {
        if (execl(program.c_str(), program.c_str(), nullptr) == -1) {
            perror("execl");
            std::exit(EXIT_FAILURE);
        }
    }
    void runParallel(const std::string& program) {
        // Get the number of hardware threads available
        unsigned int numThreads = std::thread::hardware_concurrency();
        std::cout << "Number of CPU cores: " << numThreads << std::endl;

        std::vector<std::thread> threads;

        // Atomic flag to track if any thread failed
        std::atomic<bool> hasFailed(false);

        // Create and start threads to run Geekbench
        for (unsigned int i = 0; i < numThreads; ++i) {
            threads.push_back(std::thread([&program, &hasFailed]() {
                int result = program_start(program);
                if (result != 0) {
                    std::cerr << "Thread failed with result: " << result << std::endl;
                    hasFailed.store(true);
                }
            }));
        }

        // Wait for all threads to finish
        for (auto& thread : threads) {
            thread.join();
        }

        if (!hasFailed.load()) {
            std::cout << "All threads have completed running successfully." << std::endl;
        } else {
            std::cerr << "One or more threads failed while running '" << program << "'." << std::endl;
        }
    }
}
namespace exec_tools {
    bool runBinary(const std::string& binaryPath) {
        pid_t pid;
        char* const argv[] = { const_cast<char*>(binaryPath.c_str()), nullptr };
        posix_spawn_file_actions_t actions;
        posix_spawn_file_actions_init(&actions);
        posix_spawnattr_t attr;
        posix_spawnattr_init(&attr);

        int result = posix_spawn(&pid, binaryPath.c_str(), &actions, &attr, argv, nullptr);
        if (result == 0) {
            // Wait for the child process to finish
            int status;
            if (waitpid(pid, &status, 0) == -1) {
                std::cerr << "Error waiting for the child process." << std::endl;
                return false;
            }
            return true; // Execution succeeded
        } else {
            std::cerr << "Error spawning the child process." << std::endl;
            return false; // Execution failed
        }
    }
    int Binary_with_std_system(const std::string& binaryPath) {
        std::string command = binaryPath;                   // Construct the command to execute the binary
        int exitCode = std::system(command.c_str());        // Execute the binary using system() and capture the exit code
        return exitCode;
    }
    std::future<void> runBinaryAsync(const std::string& binaryPath) {
        return std::async(std::launch::async, [&binaryPath] {
            pid_t pid;
            char* const argv[] = { const_cast<char*>(binaryPath.c_str()), nullptr };

            if (posix_spawn(&pid, binaryPath.c_str(), nullptr, nullptr, argv, nullptr) != 0) {
                // Handle the error, e.g., log an error message
                std::cerr << "Error spawning child process." << std::endl;
                return;
            }

            // Optionally, you can collect the exit status later
            int status;
            waitpid(pid, &status, 0);

            std::cout << "Child process completed." << std::endl;
        });
    }
    std::future<void> runBinaryAsync_no_error(const std::string& binaryPath) {
        return std::async(std::launch::async, [&binaryPath] {
            pid_t pid;
            char* const argv[] = { const_cast<char*>(binaryPath.c_str()), nullptr };

            if (posix_spawn(&pid, binaryPath.c_str(), nullptr, nullptr, argv, nullptr) == 0) {
                // Wait for the child process to finish
                int status;
                waitpid(pid, &status, 0);
            }
        });
    }
    std::future<void> runBinaryAsync_no_error_max_prio(const std::string& binaryPath) {
        return std::async(std::launch::async, [&binaryPath] {
            pid_t pid;
            char* const argv[] = { const_cast<char*>(binaryPath.c_str()), nullptr };
            char* const envp[] = { nullptr };

            posix_spawn_file_actions_t actions;
            posix_spawnattr_t attr;
            posix_spawnattr_setschedpolicy(&attr, SCHED_FIFO);

            // Set CPU affinity to the maximum available CPU
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(CPU_COUNT(&cpuset) - 1, &cpuset); // Set affinity to the last CPU core
            sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);

            // Set process priority to the maximum
            setpriority(PRIO_PROCESS, 0, -20); // Lower values indicate higher priority

            if (posix_spawn(&pid, binaryPath.c_str(), &actions, &attr, argv, envp) == 0) {
                // Wait for the child process to finish
                int status;
                waitpid(pid, &status, 0);
            }
        });
    }
    std::future<void> runBinaryAsync_use_all_cores(const std::string& binaryPath) {
        return std::async(std::launch::async, [&binaryPath] {
            pid_t pid;
            char* const argv[] = { const_cast<char*>(binaryPath.c_str()), nullptr };
            char* const envp[] = { nullptr };

            posix_spawn_file_actions_t actions;
            posix_spawn_file_actions_init(&actions);
            posix_spawnattr_t attr;
            posix_spawnattr_init(&attr);

            // Get the number of available CPU cores
            int numCores = std::thread::hardware_concurrency();

            // Set CPU affinity for all available cores
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            for (int i = 0; i < numCores; ++i) {
                CPU_SET(i, &cpuset);
            }
            sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);

            // Set process priority to the maximum
            setpriority(PRIO_PROCESS, 0, -20); // Lower values indicate higher priority

            if (posix_spawn(&pid, binaryPath.c_str(), &actions, &attr, argv, envp) == 0) {
                // Wait for the child process to finish
                int status;
                waitpid(pid, &status, 0);
            }
        });
    }
}
void blue_bright_string(std::string& targetstring, const std::string& searchString) {
            bool inside_quotes = false;
            std::string result; // Initialize an empty result string
            size_t pos = 0; // Start searching from the beginning of the targetstring

            while (pos < targetstring.length()) {
                // Find the next occurrence of the searchString
                size_t foundPos = targetstring.find(searchString, pos);

                if (foundPos == std::string::npos) {
                    // If the searchString is not found, copy the rest of the string
                    result += targetstring.substr(pos);
                    break;
                }

                // Check if the searchString is enclosed in quotes
                bool in_quotes = false;
                size_t quote_start = targetstring.rfind('"', foundPos);
                size_t quote_end = targetstring.find('"', foundPos + searchString.length());
                if (quote_start != std::string::npos && quote_end != std::string::npos && quote_start < foundPos && quote_end > foundPos) {
                    in_quotes = true;
                }

                // Check if we're not inside quotes before applying blue color
                if (!inside_quotes && !in_quotes) {
                    result += targetstring.substr(pos, foundPos - pos); // Copy text before the searchString
                    result += "\033[94m"; // Set text color to blue
                    result += searchString; // Copy the searchString in blue
                    result += "\033[0m"; // Reset text color to default
                } else {
                    // If the searchString is in quotes or we are inside quotes, just copy the substring as is
                    result += targetstring.substr(pos, foundPos - pos + searchString.length());
                }

                pos = foundPos + searchString.length(); // Move the position past the searchString

                // Toggle inside_quotes flag based on the presence of double quotes
                if (foundPos > 0 && targetstring[foundPos - 1] == '"') {
                    inside_quotes = !inside_quotes;
                }
            }

            // Update the targetstring with the modified result
            targetstring = result;
}
void yellow_bright_string(std::string& targetstring, const std::string& searchString) {
    bool inside_quotes = false;
    std::string result; // Initialize an empty result string
    size_t pos = 0; // Start searching from the beginning of the targetstring

    while (pos < targetstring.length()) {
        // Find the next occurrence of the searchString
        size_t foundPos = targetstring.find(searchString, pos);

        if (foundPos == std::string::npos) {
            // If the searchString is not found, copy the rest of the string
            result += targetstring.substr(pos);
            break;
        }

        if (!inside_quotes) {
            // If not inside quotes, apply blue color
            result += targetstring.substr(pos, foundPos - pos); // Copy text before the searchString
            result += "\033[94m"; // Set text color to blue
            result += searchString; // Copy the searchString in blue
            result += "\033[0m"; // Reset text color to default
        } else {
            // If inside quotes, just copy the substring as is
            result += targetstring.substr(pos, foundPos - pos + searchString.length());
        }

        pos = foundPos + searchString.length(); // Move the position past the searchString
        inside_quotes = !inside_quotes; // Toggle inside_quotes flag
    }

    // Update the targetstring with the modified result
    targetstring = result;
}
void green_char_test_old(std::string& targetstring) {
    bool inside_quotes = false;
    bool made_change = false;
    bool unsafe = true;
    std::string result; // Initialize an empty result string

    for (size_t i = 0; i < targetstring.length(); ++i) {
        if (targetstring[i] == '"') {
            if (!inside_quotes) {
                // Entering a quoted section
                inside_quotes = true;
            } else {
                // Exiting a quoted section
                inside_quotes = false;
            }
        }
        if (targetstring[i] == '3' || targetstring[i] == ' ' || targetstring[i] == 'm' || targetstring[i] == '.' || targetstring[i] == '#' || targetstring[i] == 'd') {
            unsafe = true;
        } else if (targetstring[i] == 's') {
            unsafe = false;
        }
        if (made_change) {
            made_change = false;
        } else if (targetstring[i] == '[' && !inside_quotes && unsafe == false){
            result += "\033[32m[\033[0m";
            made_change = true;
            unsafe = true;
        }
        if (!made_change) {
            result += targetstring[i];
        }
    }
    targetstring = result;                          // Update the targetstring with the modified result
}
void green_char_test(std::string& targetstring) {
    bool inside_quotes = false;
    bool test = false;
    int length = 0;
    std::string result; // Initialize an empty result string

    for (size_t i = 0; i < targetstring.length(); ++i) {
        if (targetstring[i] == '"') {
            if (!inside_quotes) {
                // Entering a quoted section
                inside_quotes = true;
            } else {
                // Exiting a quoted section
                inside_quotes = false;
            }
        }
        if (targetstring[i] == '[' && !test) {
            length = 0;
            test = true;
        }
        if (test && targetstring[i] == 'm') {
            test = false;
        }

        if (targetstring[i] == ']' && !inside_quotes) {
            result.insert(result.length() - length, "\033[32m");
            test = false;
        }
        length = length + 1;
        result += targetstring[i];
    }
    targetstring = result;                          // Update the targetstring with the modified result
}
void green(const std::string& word_to_color, std::string& in_string){
    string_manipulation::eff_replace_string(in_string, word_to_color, "\033[32m" + word_to_color + "\033[0m");
}
void yellow_bright(const std::string& word_to_color, std::string& in_string){
    string_manipulation::eff_replace_string(in_string, word_to_color, "\033[93m" + word_to_color + "\033[0m");
}
void purple(const std::string& word_to_color, std::string& in_string){
    string_manipulation::eff_replace_string(in_string, word_to_color, "\033[35m" + word_to_color + "\033[0m");
}
void blue_bright(const std::string& word_to_color, std::string& in_string){
    string_manipulation::eff_replace_string(in_string, word_to_color, "\033[94m" + word_to_color + "\033[0m");
}
void blue_dark(const std::string& word_to_color, std::string& in_string){
    string_manipulation::eff_replace_string(in_string, word_to_color, "\033[34m" + word_to_color + "\033[0m");
}
void red_between_quotes(std::string& targetstring) {
    bool inside_quotes = false;
    std::string result; // Initialize an empty result string

    for (size_t i = 0; i < targetstring.length(); ++i) {
        if (targetstring[i] == '"') {
            if (!inside_quotes) {
                // Entering a quoted section
                inside_quotes = true;
                result += "\033[31m"; // Start red text
            } else {
                ++i;
                // Exiting a quoted section
                inside_quotes = false;
                result += "\"\033[0m";               // End red text
            }
        }
        result += targetstring[i];
    }

    // Update the targetstring with the modified result
    targetstring = result;
}
void nums_outside_quotes(std::string& targetstring) {
    bool inside_quotes = false;
    bool is_num = false;
    std::string result; // Initialize an empty result string

    for (size_t i = 0; i < targetstring.length(); ++i) {
        if (targetstring[i] == '"') {
            if (!inside_quotes) {
                // Entering a quoted section
                inside_quotes = true;
            } else {
                // Exiting a quoted section
                inside_quotes = false;
            }
        } else if ((targetstring[i] >= '0' && targetstring[i] <= '9') && !inside_quotes) {
            if (!is_num){
                is_num = true;
                result += "\033[33m";
            }
        } else if (is_num) {
            result += "\033[0m";
            is_num = false;
        }
        result += targetstring[i];
    }

    // Update the targetstring with the modified result
    targetstring = result;
}
void namespace_functions(std::string& targetstring) {
    bool inside_quotes = false;
    bool last_colon = false;
    bool found_colon = false;
    std::string result; // Initialize an empty result string
    int length;

    for (size_t i = 0; i < targetstring.length(); ++i) {
        if (targetstring[i] == ' ' && found_colon){
            result += "\033[0m";
        }
        if (targetstring[i] == '"') {
            if (!inside_quotes) {
                // Entering a quoted section
                inside_quotes = true;
            } else {
                // Exiting a quoted section
                inside_quotes = false;
            }
        }
        if (targetstring[i] == ':' && !inside_quotes) {
            if (!found_colon){
                found_colon = true;
                last_colon = false;
                result += "\033[32m";
            } else {
                found_colon = false;
                last_colon = true;
                length = 1;
                ++i;
                result += ":\033[0m";
            }
        } else if (targetstring[i] == '(' && last_colon == true) {
            result.insert(result.length() - length, "\033[35m");
            result += "\033[0m";
        } else if (last_colon) {
            if (targetstring[i] == ' ') {
                last_colon = false;
                length = 0;
            } else {
                length = length + 1;
            }
        }
        result += targetstring[i];
    }

    // Update the targetstring with the modified result
    targetstring = result;
}
void functions(std::string& targetstring) {
    std::string result; // Initialize an empty result string
    int length;

    for (size_t i = 0; i < targetstring.length(); ++i) {
        if (targetstring[i] == ' ') {
            length = -1;
        } else if (targetstring[i] == '.'){
            length = -1;
        } else if (targetstring[i] == ':'){
            length = -1;
        } else {
            length = length + 1;
        }
        if (targetstring[i] == '(' && length >= 2) {
            result.insert(result.length() - length, "\033[35m");
            result += "\033[0m";
        }
        result += targetstring[i];
    }

    // Update the targetstring with the modified result
    targetstring = result;
}
void green_char(std::string& targetstring, const char c) {
    bool inside_quotes = false;
    bool found = false;
    std::string result; // Initialize an empty result string

    for (size_t i = 0; i < targetstring.length(); ++i) {
        if (targetstring[i] == '"') {
            if (!inside_quotes) {
                // Entering a quoted section
                inside_quotes = true;
            } else {
                // Exiting a quoted section
                inside_quotes = false;
            }
        }
        if (found){
            result += "\033[0m";
            found = false;
        }
        if (targetstring[i] == c && !inside_quotes){
            result += "\033[32m";
            found = true;
        }
        result += targetstring[i];
    }

    // Update the targetstring with the modified result
    targetstring = result;
}
 void testfgg(std::string& targetstring){
            std::regex string("\\bstring\\b");
            std::regex quote("\"");
            bool inside_quotes = false;
            std::string word;
            std::istringstream iss(targetstring);
            std::string line;
            int lineNum = 1;
            int quote_num = 0;
            bool end_qoute = true;
            std::vector<std::string> quote_p;

            while (std::getline(iss, line)) {               // Loop through lines of the input string
                std::smatch match;
                std::string::const_iterator searchStart(line.cbegin());
                while (std::regex_search(searchStart, line.cend(), match, quote)) {// Search for the whole word in the line
                    quote_num++;
                    quote_p.push_back(std::to_string(quote_num) + " line="+std::to_string(lineNum)+"pos="+std::to_string(match.position()));
                    searchStart = match.suffix().first;
                }
                lineNum++;
            }
            while (iss >> word){
                if (word.find('"') != std::string::npos && !inside_quotes){
                    inside_quotes = true;
                } else if (word.find('"') != std::string::npos && !inside_quotes){
                    inside_quotes = false;
                } else if (!inside_quotes && word == "string"){
                    targetstring = std::regex_replace(targetstring, string, "\e[94m$&\e[0m");
                }
            }
        }

int save() {
    std::string input = "This is a string within quotes. Another example line with multiple quotes.";

    std::regex wordPattern("\\bstring\\b"); // Pattern to match the word "string"
    std::string colorStart = "\e[31m";      // ANSI escape code for red color
    std::string colorEnd = "\e[0m";          // ANSI escape code to reset color

    int targetLine = 2;  // Target line number
    int targetPos = 8;   // Target position within the line

    std::smatch match;
    std::string::const_iterator searchStart(input.cbegin());
    int currentLine = 1;  // Current line number

    while (std::regex_search(searchStart, input.cend(), match, wordPattern)) {
        // Calculate the position of the matched word in the original string
        int position = std::distance(input.cbegin(), match[0].first);

        // Check if the match is on the target line and at the target position
        if (currentLine == targetLine && position == targetPos) {
            // Extract the matched word
            std::string matchedWord = match.str();

            // Replace the matched word with the colored version
            input.replace(match.position(), matchedWord.length(), colorStart + matchedWord + colorEnd);

            // Break the loop as we have colored the word at the target line and position
            break;
        }

        // Update searchStart for the next iteration
        searchStart = match[0].second;

        // Update the current line number based on line breaks
        if (match[0].str().find('\n') != std::string::npos) {
            currentLine++;
        }
    }

    // Print the modified string with the colored word
    std::cout << input << std::endl;

    return 0;
}
void blue_char(std::string& targetstring, const char c) {
        bool inside_quotes = false;
        bool found = false;
        std::string result; // Initialize an empty result string

        for (size_t i = 0; i < targetstring.length(); ++i) {
            if (targetstring[i] == '"') {
                if (!inside_quotes) {
                    // Entering a quoted section
                    inside_quotes = true;
                } else {
                    // Exiting a quoted section
                    inside_quotes = false;
                }
            }
            if (found){
                result += "\033[0m";
                found = false;
            }
            if (targetstring[i] == c && !inside_quotes){
                result += "\033[94m";
                found = true;
            }
            result += targetstring[i];
        }

        // Update the targetstring with the modified result
        targetstring = result;
    }
void quotes_blue(std::string& targetstring){
    std::regex string("\\bstring\\b");
    std::regex quote("\"");
    std::regex quotePattern("\'.*?\'");
    // bool inside_quotes = false;
    std::string word;
    std::istringstream iss(targetstring);
    std::string line;
    int lineNum = 1;
    int quote_num = 0;
    int length = 0;
    int last_line_num = 0;

    while (std::getline(iss, line)) {               // Loop through lines of the input string
        std::smatch match;
        std::string::const_iterator searchStart(line.cbegin());
        while (std::regex_search(searchStart, line.cend(), match, quotePattern)) {// Search for the whole word in the line
            if (last_line_num != lineNum){
                length = 0;
            } else {
                length = length + 9;
            }
            length = length + match.position() + 1;
            quote_num++;
            std::cout << "start :quote :" << quote_num << " line :" << lineNum << ":" << length << ":" << length + match.length() << "\n";
            // insertStringAtPosition(targetstring, "\e[31m", lineNum, length - 1);
            // insertStringAtPosition(targetstring, "\e[0m", lineNum, length + match.length() + 4);
            length = length + 1;
            searchStart = match.suffix().first;
            last_line_num = lineNum;
            length = length + match.length() - 2;
        }
        lineNum++;
    }
    // while (iss >> word){
    //     if (word.find('"') != std::string::npos && !inside_quotes){
    //         inside_quotes = true;
    //     } else if (word.find('"') != std::string::npos && !inside_quotes){
    //         inside_quotes = false;
    //     } else if (!inside_quotes && word == "string"){
    //         targetstring = std::regex_replace(targetstring, string, "\e[94m$&\e[0m");
    //     }
    // }
}
void string_blue(std::string& targetstring){
    std::regex string("\\bstring\\b(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)");
    // bool inside_quotes = false;
    std::string word;
    std::istringstream iss(targetstring);
    std::string line;
    int lineNum = 1;
    int quote_num = 0;
    int length = 0;
    int last_line_num = 0;

    while (std::getline(iss, line)) {               // Loop through lines of the input string
        std::smatch match;
        std::string::const_iterator searchStart(line.cbegin());
        while (std::regex_search(searchStart, line.cend(), match, string)) {// Search for the whole word in the line
            if (last_line_num != lineNum){
                length = 0;
            } else {
                length = length + 9;
            }
            length = length + match.position() + 1;
            quote_num++;
            std::cout << "start :string :" << quote_num << " line :" << lineNum << ":" << length << ":" << length + match.length() << "\n";
            // insertStringAtPosition(targetstring, "\e[94m", lineNum, length - 1);
            // insertStringAtPosition(targetstring, "\e[0m", lineNum, length + match.length() + 4);
            length = length + 1;
            searchStart = match.suffix().first;
            last_line_num = lineNum;
            length = length + match.length() - 2;
        }
        lineNum++;
    }
    // while (iss >> word){
    //     if (word.find('"') != std::string::npos && !inside_quotes){
    //         inside_quotes = true;
    //     } else if (word.find('"') != std::string::npos && !inside_quotes){
    //         inside_quotes = false;
    //     } else if (!inside_quotes && word == "string"){
    //         targetstring = std::regex_replace(targetstring, string, "\e[94m$&\e[0m");
    //     }
    // }
}
void comments_gray(std::string& targetstring){
    std::regex string("\\/\\/.*$");
    // bool inside_quotes = false;
    std::string word;
    std::istringstream iss(targetstring);
    std::string line;
    int lineNum = 1;
    int quote_num = 0;
    int length = 0;
    int last_line_num = 0;

    while (std::getline(iss, line)) {               // Loop through lines of the input string
        std::smatch match;
        std::string::const_iterator searchStart(line.cbegin());
        while (std::regex_search(searchStart, line.cend(), match, string)) {// Search for the whole word in the line
            if (last_line_num != lineNum){
                length = 0;
            } else {
                length = length + 9;
            }
            length = length + match.position() + 1;
            quote_num++;
            std::cout << "start :string :" << quote_num << " line :" << lineNum << ":" << length << ":" << length + match.length() << "\n";
            // insertStringAtPosition(targetstring, "\e[90m", lineNum, length - 1);
            // insertStringAtPosition(targetstring, "\e[0m", lineNum, length + match.length() + 4);
            length = length + 1;
            searchStart = match.suffix().first;
            last_line_num = lineNum;
            length = length + match.length() - 2;
        }
        lineNum++;
    }
    // while (iss >> word){
    //     if (word.find('"') != std::string::npos && !inside_quotes){
    //         inside_quotes = true;
    //     } else if (word.find('"') != std::string::npos && !inside_quotes){
    //         inside_quotes = false;
    //     } else if (!inside_quotes && word == "string"){
    //         targetstring = std::regex_replace(targetstring, string, "\e[94m$&\e[0m");
    //     }
    // }
}
