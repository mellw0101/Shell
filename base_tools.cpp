#include "base_tools.h"

namespace fs = std::filesystem;
void error_message(const std::string& program, const std::string& message) {
        std::cerr << program + ": ERROR: " << message << " (press enter to continue)";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void error_message_no_halt(const std::string& program, const std::string& message) {
    std::cerr << program << ": \033[31mERROR\033[0m: " << message << "\n";
}
void set_env_var() {
    const char* pathEnv = std::getenv("PATH");
    if (pathEnv != nullptr) {
        setenv("PATH", pathEnv, 1);
    }
    const char* displayEnv = std::getenv("DISPLAY");
    if (displayEnv != nullptr) {
        setenv("DISPLAY", displayEnv, 1);
    }
    const char* XAUTHORITYenv = std::getenv("XAUTHORITY");
    if (XAUTHORITYenv != nullptr) {
        setenv("XAUTHORITY", "/tmp/xauth_tcvJLb", 1);
    }
}
bool check_extension(const std::string& filename, const std::string& extension){
    std::string check = filename.substr(filename.length() - extension.length());
    if (check != extension) {
        return false;
    }
    return true;
}
namespace get_vars {
    std::string get_PATH_var() {
        const char* env_var = std::getenv("PATH");
        if (env_var == nullptr) {
            // Handle the case where PATH is not set or is empty
            return "";
        }
        return std::string(env_var);
    }
    std::string get_XAUTHORITY_var() {
        const char* env_var = std::getenv("XAUTHORITY");
        if (env_var == nullptr) {
            // Handle the case where PATH is not set or is empty
            return "";
        }
        return std::string(env_var);
    }
    namespace get_USER {
        std::string get_USER_var() {
            const char* env_var = std::getenv("USER");
            if (env_var == nullptr) {
                // Handle the case where PATH is not set or is empty
                return "";
            }
            return std::string(env_var);
        }
        std::string getUserName() {
            uid_t uid = getuid(); // Get the user ID of the calling process
            struct passwd *pw = getpwuid(uid);
            if (pw) {
                return std::string(pw->pw_name); // Return the username
            } else {
                return std::string("unknown"); // Return "unknown" if the username couldn't be found
            }
        }
        std::string getUserName_passwd_file() {
            std::ifstream passwdFile("/etc/passwd");
            std::string line;
            uid_t uid = getuid(); // Get the user ID of the calling process

            while (std::getline(passwdFile, line)) {
                std::stringstream ss(line);
                std::string segment;
                std::vector<std::string> fields;

                while (std::getline(ss, segment, ':')) {
                    fields.push_back(segment);
                }

                if (fields.size() >= 3 && std::stoi(fields[2]) == uid) {
                    return fields[0]; // Return the username
                }
            }

            return "unknown"; // If not found
        }
    }
    std::string get_DISPLAY_var() {
        const char* env_var = std::getenv("DISPLAY");
        if (env_var == nullptr) {
            // Handle the case where PATH is not set or is empty
            return "";
        }
        return std::string(env_var);
    }
    std::string get_HOME_var() {
        const char* env_var = std::getenv("HOME");
        if (env_var == nullptr) {
            // Handle the case where PATH is not set or is empty
            return "";
        }
        return std::string(env_var);
    }
    std::string get_TERM_var() {
        const char* env_var = std::getenv("TERM");
        if (env_var == nullptr) {
            // Handle the case where PATH is not set or is empty
            return "";
        }
        return std::string(env_var);

    }
    std::string get_COLORTERM_var() {
        const char* env_var = std::getenv("COLORTERM");
        if (env_var == nullptr) {
            // Handle the case where PATH is not set or is empty
            return "";
        }
        return std::string(env_var);
    }
    std::string get_XDG_CURRENT_DESKTOP_var() {
        const char* env_var = std::getenv("XDG_CURRENT_DESKTOP");
        if (env_var == nullptr) {
            // Handle the case where PATH is not set or is empty
            return "";
        }
        return std::string(env_var);
    }
    std::string get_DPCPP_HOME_var() {
        const char* env_var = std::getenv("DPCPP_HOME");
        if (env_var == nullptr) {
            // Handle the case where PATH is not set or is empty
            return "";
        }
        return std::string(env_var);
    }
    std::string get_DBUS_SESSION_BUS_ADDRESS_var() {
        const char* env_var = std::getenv("DBUS_SESSION_BUS_ADDRESS");
        if (env_var == nullptr) {
            // Handle the case where PATH is not set or is empty
            return "";
        }
        return std::string(env_var);
    }
    std::string get_XDG_CONFIG_DIRS_var() {
        const char* env_var = std::getenv("XDG_CONFIG_DIRS");
        if (env_var == nullptr) {
            // Handle the case where PATH is not set or is empty
            return "";
        }
        return std::string(env_var);
    }
    std::string get_XDG_SEAT_var() {
        const char* env_var = std::getenv("XDG_SEAT");
        if (env_var == nullptr) {
            // Handle the case where PATH is not set or is empty
            return "";
        }
        return std::string(env_var);
    }
    std::string get_XDG_SESSION_CLASS_var() {
        const char* env_var = std::getenv("XDG_SESSION_CLASS");
        if (env_var == nullptr) {
            // Handle the case where PATH is not set or is empty
            return "";
        }
        return std::string(env_var);
    }
    std::string get_XDG_RUNTIME_DIR_var() {
        const char* env_var = std::getenv("XDG_RUNTIME_DIR");
        if (env_var == nullptr) {
            // Handle the case where PATH is not set or is empty
            return "";
        }
        return std::string(env_var);
    }
    std::string get_CURRENT_WORKING_DIRECTORY_var() {
        return fs::current_path().string();
    }
}
namespace args {
    bool find_arg(const std::vector<std::string>& args, const std::string& flag) {
        for (size_t i = 0; i < args.size(); ++i) {
            if (args[i] == flag) {
                return true;
            }
        }
        return false;
    }
    std::string processArgs(const std::vector<std::string>& args, const std::string& flag) {
        for (size_t i = 0; i < args.size(); ++i) {
            if (args[i] == flag) {
                // When the "-flag" argument is found, do something with the argument after it
                if (i + 1 < args.size()) {
                    const std::string& argAfterFlag = args[i + 1];
                    return argAfterFlag;
                } else {
                    return "";
                }
            }
        }
        return "";
    }
    std::string processArgs_2(const std::vector<std::string>& args, const std::string& flag) {
        for (size_t i = 0; i < args.size(); ++i) {
            if (args[i] == flag) {
                // When the "-flag" argument is found, do something with the argument after it
                if (i + 1 < args.size()) {
                    const std::string& argAfterFlag = args[i + 2];
                    return argAfterFlag;
                } else {
                    return "";
                }
            }
        }
        return "";
    }
    std::vector<std::string> combineArgsBetweenQuotes(const std::vector<std::string>& input) {
        std::vector<std::string> result;
        bool insideQuotes = false;
        std::string combinedArg;

        for (const std::string& arg : input) {
            if (arg.front() == '"' && arg.back() == '"') {
                result.push_back(arg.substr(1, arg.size() - 2));
            } else if (arg.front() == '"' && !insideQuotes) {
                // Start of a quoted argument
                insideQuotes = true;
                combinedArg = arg.substr(1);
            } else if (arg.back() == '"' && insideQuotes) {
                // End of a quoted argument
                combinedArg += " " + arg.substr(0, arg.size() - 1);
                result.push_back(combinedArg);
                insideQuotes = false;
            } else if (insideQuotes) {
                // Inside a quoted argument, concatenate it
                combinedArg += " " + arg;
            } else {
                // Not inside quotes, add as is
                result.push_back(arg);
            }
        }
        return result;
    }
}
namespace tools {
    void inputF(const std::string& input, const std::string& full_path) {
        if (fs::is_directory(full_path)) {
            error_message("input_to_file", "'" + full_path + "' is a directory not a file");
            return;
        }
        if (!fs::exists(full_path)) {
            error_message("input_to_file", "target file '" + full_path + "' does not exist");
            return;
        }
        std::ofstream openFILE;
        openFILE.open(full_path, std::ios::app);
        if (!openFILE.is_open()) {
            error_message_no_halt("input_to_file", "can't open '" + full_path + "'");
            return;
        }
        openFILE << input;
        openFILE.close();
    }
    bool rm_f(const std::string& filePath) {
        if (std::remove(filePath.c_str()) != 0) {
            error_message_no_halt("rm_f", "could not delete file '" + filePath + "'");
            return false;
        }
        return true;
    }
    void create_file(const std::string& full_PATH_to_file) {
        if (fs::is_directory(full_PATH_to_file)) {
            error_message_no_halt("create_file", "target file '" + full_PATH_to_file + "' is a directory");
            return;
        }
        if (fs::exists(full_PATH_to_file)) {
            error_message_no_halt("create_file", "target file '" + full_PATH_to_file + "' already exists");
            return;
        }
        std::ofstream outFile;
        outFile.open(full_PATH_to_file, std::ios::out | std::ios::trunc);
        if (!outFile.is_open()) {
            error_message("create_file", "Failed to create a new file at target '" + full_PATH_to_file + "'");
            return;
        }
        outFile.close();
    }
    void cddir(const std::string& full_PATH_to_dir) {   // move active directory to target
        if (!fs::exists(full_PATH_to_dir)) {
            error_message_no_halt("cddir", "'" + full_PATH_to_dir + "' dosent exist.");
            return;
        }
        if (!fs::is_directory(full_PATH_to_dir)) {
            error_message_no_halt("cddir", "'" + full_PATH_to_dir + "' is a file.");
            return;
        }
        if (chdir(full_PATH_to_dir.c_str()) != 0) {
            error_message_no_halt("cddir", "Failed to change directory.");
            return;
        }
    }
    void cmkdir(const std::string& full_PATH_to_dir) {          // make dir if one does not exist at target
        if (fs::is_directory(full_PATH_to_dir)) {
            error_message_no_halt("cmkdir", "Directory already exists.");
            return;
        }
        if (fs::exists(full_PATH_to_dir)) {
            error_message_no_halt("cmkdir", "'" + full_PATH_to_dir + "' is a file");
            return;
        }
        try {
            fs::create_directory(full_PATH_to_dir);
        }
        catch (const fs::filesystem_error& e) {
            error_message_no_halt("cmkdir", e.what());
        }
    }
    bool c_mv(const fs::path& source, const fs::path& destination) {
        std::error_code ec;  // For capturing any error messages
        if (!fs::exists(source, ec)) {
            error_message_no_halt("c_mv", "Source path does not exist.");
            return false;
        }
        if (fs::exists(destination, ec)) {
            error_message_no_halt("c_mv", "Destination path already exists.");
            return false;
        }
        try {
            fs::rename(source, destination);
        } catch (const std::filesystem::filesystem_error& e) {
            error_message_no_halt("c_mv", e.what());
            return false;
        }
        return true;
    }
}
std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = input.find(delimiter);
    while (end != std::string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + 1;
        end = input.find(delimiter, start);
    }
    tokens.push_back(input.substr(start, end));
    return tokens;
}
std::string search_in_PATH(const std::string& phrase) {
        std::string pathEnv = get_vars::get_PATH_var();

        std::vector<std::string> paths = splitString(pathEnv, ':');
        std::string fullPath;

        bool found = false;
        for (const std::string& path : paths) {
            // Check if the phrase exists in the current path
            fullPath = path + "/" + phrase;
            if (access(fullPath.c_str(), F_OK) == 0) {
                // std::cout << "Found in path: " << path << std::endl;
                found = true;
                break;
            }
        }

        if (!found) {
            return "Command not found in any PATH directory.";
        }

        return fullPath;
}
void itf(const std::vector<std::string>& args) {
    if (args.size() != 3) {
        std::cout << "Usage: inputF <input> <full_path_to_target_file>" << std::endl;
        return;
    }
    std::string input = args[1];
    std::string full_path = args[2];
    tools::inputF(input, full_path);
}
void cf(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cout << "Usage: cf <filename> 'to make in current directory' <full_PATH_to_file> 'to make at target'\n";
        return;
    }
    tools::create_file(args[1]);
}
void cd(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        std::string home = get_vars::get_HOME_var();
        tools::cddir(home);
    } else if (args.size() == 2) {
        std::string full_PATH_to_dir = args[1];
        tools::cddir(full_PATH_to_dir);
    } else if (args.size() == 3) {
        std::string full_PATH_to_dir = args[1] + " " + args[2];
        tools::cddir(full_PATH_to_dir);
    } else {
        std::cerr << "Usage: cd <full_path_to_target_dir>\n";
    }
}
void cmkdir(const std::vector<std::string>& args) {
    if (args.size() == 2) {
        tools::cmkdir(args[1]);
    }
}
void mv(const std::vector<std::string>& args) {
    if (!args::find_arg(args, args::processArgs(args, "mv"))) {
        error_message_no_halt("mv", "no source given.");
        return;
    }
    if (!args::find_arg(args, args::processArgs_2(args, "mv"))) {
        error_message_no_halt("mv", "no destination given.");
        return;
    }
    if (tools::c_mv(args::processArgs(args, "mv"), args::processArgs_2(args, "mv"))) {
        return;
    }
}
int stringToInt(const std::string& str) {
    try {
        int value = std::stoi(str);
        return value;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Conversion failed: Invalid argument - " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Conversion failed: Out of range - " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Conversion failed: Unknown error" << std::endl;
    }
    return 0;  // Return 0 on failure
}
void listFiles(const std::string& directoryPath) {
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(
                directoryPath,
                std::filesystem::directory_options::skip_permission_denied))
        {
            std::cout << entry.path() << '\n';
        }
    } catch(const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << '\n';
    }
}
bool bash(const std::string& command, const std::string& input) {
    FILE* pipe = popen(command.c_str(), "w");
    if (!pipe) return false;
        fprintf(pipe, "%s", input.c_str());
        fflush(pipe);
        pclose(pipe);
        return true;
}

// Function to get the terminal size (rows and columns)
void getTerminalSize(int& rows, int& cols) {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    rows = ws.ws_row;
    cols = ws.ws_col;
}
void getTerminalCols(int& cols) {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    cols = ws.ws_col;
}
void termsize() {
    int rows, cols;
    getTerminalSize(rows, cols);

    std::cout << "Terminal Size: Rows = " << rows << ", Columns = " << cols << std::endl;
}
int colums() {
    int cols;
    getTerminalCols(cols);
    return cols;
}

char** vectorToArgv(const std::vector<std::string>& vec) {
    char** argv = new char*[vec.size() + 1];  // Allocate memory for argv array. +1 for the nullptr terminator.
    for (size_t i = 0; i < vec.size(); ++i) {
        argv[i] = const_cast<char*>(vec[i].c_str());  // Store pointer to string's character data in argv array.
    }
    argv[vec.size()] = nullptr;  // Add nullptr terminator to argv array.
    return argv;
}
void cleanUpArgv(char** argv) {
    delete[] argv;  // Deallocate memory for argv array.
}
