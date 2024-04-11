#ifndef BASE_TOOLS_H
#define BASE_TOOLS_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <filesystem>
#include <fstream>
#include <set>
#include <unistd.h> // for getuid
#include <pwd.h> // for getpwuid
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

namespace fs = std::filesystem;

void error_message(const std::string& program, const std::string& message);
void error_message_no_halt(const std::string& program, const std::string& message);
void set_env_var();
bool check_extension(const std::string& filename, const std::string& extension);
namespace get_vars {
    std::string get_PATH_var();
    std::string get_XAUTHORITY_var();
    namespace get_USER {
        std::string get_USER_var();
        std::string getUserName();
    }
    std::string get_DISPLAY_var();
    std::string get_HOME_var();
    std::string get_TERM_var();
    std::string get_COLORTERM_var();
    std::string get_XDG_CURRENT_DESKTOP_var();
    std::string get_DPCPP_HOME_var();
    std::string get_DBUS_SESSION_BUS_ADDRESS_var();
    std::string get_XDG_CONFIG_DIRS_var();
    std::string get_XDG_SEAT_var();
    std::string get_XDG_SESSION_CLASS_var();
    std::string get_XDG_RUNTIME_DIR_var();
    std::string get_CURRENT_WORKING_DIRECTORY_var();
}
namespace args {
    bool find_arg(const std::vector<std::string>& args, const std::string& flag);
    std::string processArgs(const std::vector<std::string>& args, const std::string& flag);
    std::string processArgs_2(const std::vector<std::string>& args, const std::string& flag);
    std::vector<std::string> combineArgsBetweenQuotes(const std::vector<std::string>& input);
}
namespace tools {
    void inputF(const std::string& input, const std::string& full_path);
    void create_file(const std::string& full_PATH_to_file);
    void cddir(const std::string& full_PATH_to_dir);
    bool rm_f(const std::string& filePath);
    void cmkdir(const std::string& full_PATH_to_dir);
    bool c_mv(const fs::path& source, const fs::path& destination);
}

std::vector<std::string> splitString(const std::string& input, char delimiter);
std::string search_in_PATH(const std::string& phrase);// move active directory to target
void itf(const std::vector<std::string>& args);
void cf(const std::vector<std::string>& args);
void cd(const std::vector<std::string>& args);
void cmkdir(const std::vector<std::string>& args);
void mv(const std::vector<std::string>& args);
int stringToInt(const std::string& str);
void listFiles(const std::string& directoryPath);

void termsize();
int colums();

char** vectorToArgv(const std::vector<std::string>& vec);
void cleanUpArgv(char** argv);

#endif // BASE_TOOLS_H
