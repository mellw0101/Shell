#include "base_tools.h"
#include "run.h"


namespace tools {
    std::string dll(const std::string& full_parh_to_dll){
        return "WINEDLLOVERRIDES=dllname=native,\"" + full_parh_to_dll + "\"";
    }
}

std::future<void> wine(const std::vector<std::string> &args) {
    return std::async(std::launch::async, [&args] {
        std::string XAUTHORITY = "XAUTHORITY=" + get_vars::get_XAUTHORITY_var();
        std::string PATH = "PATH=" + get_vars::get_PATH_var();
        std::string DISPLAY = "DISPLAY=" + get_vars::get_DISPLAY_var();
        std::string TERM = "TERM=" + get_vars::get_TERM_var();
        std::string COLORTERM = "COLORTERM=" + get_vars::get_COLORTERM_var();
        std::string HOME = "HOME=" + get_vars::get_HOME_var();
        std::string USER = "USER=" + get_vars::get_USER::get_USER_var();
        std::string DBUS_SESSION_BUS_ADDRESS = "DBUS_SESSION_BUS_ADDRESS=" + get_vars::get_DBUS_SESSION_BUS_ADDRESS_var();
        std::string DPCPP_HOME = "DPCPP_HOME=" + get_vars::get_DPCPP_HOME_var();
        std::string XDG_CURRENT_DESKTOP = "XDG_CURRENT_DESKTOP=" + get_vars::get_XDG_CURRENT_DESKTOP_var();
        std::string XDG_SESSION_CLASS = "XDG_SESSION_CLASS=" + get_vars::get_XDG_SESSION_CLASS_var();
        std::string XDG_CONFIG_DIRS = "XDG_CONFIG_DIRS=" + get_vars::get_XDG_CONFIG_DIRS_var();
        std::string XDG_SEAT = "XDG_SEAT=" + get_vars::get_XDG_SEAT_var();
        std::string XDG_RUNTIME_DIR = "XDG_RUNTIME_DIR=" + get_vars::get_XDG_RUNTIME_DIR_var();
        std::string WINEFSYNC = "WINEFSYNC=1";
        std::string mozglue = tools::dll("/home/mellw/.wine/drive_c/Program Files/Mozilla Firefox/mozglue.dll");
        std::string ntdll = tools::dll("/usr/lib/wine/x86_64-windows/ntdll.dll");


        std::vector<char*> argv;
        for (const std::string& arg : args) {       // Build the command string
            argv.push_back(const_cast<char*>(arg.c_str()));      // Add each argument to the argv vector
        }
        argv.push_back(nullptr);  // The last element must be nullptr

        std::string binary = "/bin/wine";
        pid_t pid;

        // Get the pointer to the argv vector
        char* const* argvPtr = argv.data();

        char* const envp[] = {
            //const_cast<char*>("XDG_SESSION_PATH=/org/freedesktop/DisplayManager/Session0"),
            //const_cast<char*>("XCURSOR_SIZE=24"),
            //const_cast<char*>("XDG_SESSION_DESKTOP=KDE"),
            //const_cast<char*>("XDG_SESSION_TYPE=x11"),
            //const_cast<char*>("XDG_SEAT_PATH=/org/freedesktop/DisplayManager/Seat0"),
            // const_cast<char*>("XCURSOR_THEME=breeze_cursors"),
            // const_cast<char*>("XDG_SESSION_ID=1"),
            const_cast<char*>(DPCPP_HOME.c_str()),
            const_cast<char*>(DBUS_SESSION_BUS_ADDRESS.c_str()),
            const_cast<char*>(COLORTERM.c_str()),
            const_cast<char*>(TERM.c_str()),
            const_cast<char*>(XDG_CONFIG_DIRS.c_str()),
            const_cast<char*>(XDG_RUNTIME_DIR.c_str()),
            const_cast<char*>(XDG_SEAT.c_str()),
            const_cast<char*>(XDG_SESSION_CLASS.c_str()),
            const_cast<char*>(XDG_CURRENT_DESKTOP.c_str()),
            const_cast<char*>(XAUTHORITY.c_str()),
            const_cast<char*>(USER.c_str()),
            const_cast<char*>(DISPLAY.c_str()),
            const_cast<char*>(HOME.c_str()),
            const_cast<char*>(PATH.c_str()),
            const_cast<char*>(WINEFSYNC.c_str()),
            const_cast<char*>(mozglue.c_str()),
            const_cast<char*>(ntdll.c_str()),
            const_cast<char*>("WINEDEBUG=+dll,-warn"),
            const_cast<char*>("WINEPREFIX=/home/mellw/.wine"),
            nullptr
        };

        static posix_spawn_file_actions_t actions;
        static posix_spawnattr_t attr;

        posix_spawn_file_actions_init(&actions);
        posix_spawnattr_init(&attr);


        int status;
        if (posix_spawn(&pid, "/bin/wine64", &actions, &attr, argvPtr, envp) == 0) {
            // Wait for the child process to finish
            waitpid(pid, &status, 0);
        } else {
            throw std::runtime_error("Error spawning the process: " + std::string(strerror(errno)));
        }
    });
}
std::future<void> runBinaryAsync_with_env(const std::string& binaryPath) {
    return std::async(std::launch::async, [&binaryPath] {
        std::string XAUTHORITY = "XAUTHORITY=" +  get_vars::get_XAUTHORITY_var();
        std::string PATH = "PATH=" +  get_vars::get_PATH_var();
        std::string DISPLAY = "DISPLAY=" +  get_vars::get_DISPLAY_var();
        std::string TERM = "TERM=" +  get_vars::get_TERM_var();
        std::string COLORTERM = "COLORTERM=" +  get_vars::get_COLORTERM_var();
        std::string HOME = "HOME=" +  get_vars::get_HOME_var();
        std::string USER = "USER=" +  get_vars::get_USER::get_USER_var();
        std::string DBUS_SESSION_BUS_ADDRESS = "DBUS_SESSION_BUS_ADDRESS=" +  get_vars::get_DBUS_SESSION_BUS_ADDRESS_var();
        std::string DPCPP_HOME = "DPCPP_HOME=" +  get_vars::get_DPCPP_HOME_var();
        std::string XDG_CURRENT_DESKTOP = "XDG_CURRENT_DESKTOP=" +  get_vars::get_XDG_CURRENT_DESKTOP_var();
        std::string XDG_SESSION_CLASS = "XDG_SESSION_CLASS=" +  get_vars::get_XDG_SESSION_CLASS_var();
        std::string XDG_CONFIG_DIRS = "XDG_CONFIG_DIRS=" +  get_vars::get_XDG_CONFIG_DIRS_var();
        std::string XDG_SEAT = "XDG_SEAT=" +  get_vars::get_XDG_SEAT_var();
        std::string XDG_RUNTIME_DIR = "XDG_RUNTIME_DIR=" + get_vars::get_XDG_RUNTIME_DIR_var();
        // steam env vars
        std::string STEAM_RUNTIME = "STEAM_RUNTIME_HEAVY=1";
        std::string RLIMIT_NICE_var = "RLIMIT_NICE=22";
        std::string STEAM_DEBUGGER = "STEAM_DEBUGGER=valgrind";
        std::string LD_PRELOAD = "LD_PRELOAD=/usr/local/lib/libgamemodeauto.so";


        pid_t pid;
        char* const argv[] = { const_cast<char*>(binaryPath.c_str()), nullptr };
        char* const envp[] = {
            // const_cast<char*>(STEAM_RUNTIME.c_str()),
            const_cast<char*>(PATH.c_str()),
            const_cast<char*>(DBUS_SESSION_BUS_ADDRESS.c_str()),
            const_cast<char*>(XDG_SEAT.c_str()),
            const_cast<char*>(XDG_RUNTIME_DIR.c_str()),
            const_cast<char*>(XDG_CURRENT_DESKTOP.c_str()),
            const_cast<char*>(XDG_CONFIG_DIRS.c_str()),
            const_cast<char*>(XDG_SESSION_CLASS.c_str()),
            const_cast<char*>(XAUTHORITY.c_str()),
            const_cast<char*>(USER.c_str()),
            const_cast<char*>(DISPLAY.c_str()),
            const_cast<char*>(HOME.c_str()),
            nullptr
        };

        posix_spawnattr_t attr;
        posix_spawn_file_actions_t actions;

        posix_spawnattr_setschedpolicy(&attr, SCHED_FIFO);

        posix_spawn_file_actions_init(&actions);
        posix_spawnattr_init(&attr);


        int status;
        if (posix_spawn(&pid, binaryPath.c_str(), nullptr, nullptr, argv, envp) == 0) {
            // Wait for the child process to finish
            waitpid(pid, &status, 0);
        } else {
            throw std::runtime_error("Error spawning the process: " + std::string(strerror(errno)));
        }
    });
}
std::future<void> runBinaryAsync_with_env_wargs(const std::string& binaryPath, const std::vector<std::string>& args) {
    return std::async(std::launch::async, [&binaryPath, &args] {
        std::string LD_PRELOAD = "LD_PRELOAD=/usr/local/lib/libgamemodeauto.so";
        std::string XAUTHORITY = "XAUTHORITY=" + get_vars::get_XAUTHORITY_var();
        std::string PATH = "PATH=" + get_vars::get_PATH_var();
        std::string DISPLAY = "DISPLAY=" + get_vars::get_DISPLAY_var();
        std::string TERM = "TERM=" + get_vars::get_TERM_var();
        std::string COLORTERM = "COLORTERM=" + get_vars::get_COLORTERM_var();
        std::string HOME = "HOME=" + get_vars::get_HOME_var();
        std::string USER = "USER=" + get_vars::get_USER::get_USER_var();
        std::string DBUS_SESSION_BUS_ADDRESS = "DBUS_SESSION_BUS_ADDRESS=" + get_vars::get_DBUS_SESSION_BUS_ADDRESS_var();
        std::string DPCPP_HOME = "DPCPP_HOME=" + get_vars::get_DPCPP_HOME_var();
        std::string XDG_CURRENT_DESKTOP = "XDG_CURRENT_DESKTOP=" + get_vars::get_XDG_CURRENT_DESKTOP_var();
        std::string XDG_SESSION_CLASS = "XDG_SESSION_CLASS=" + get_vars::get_XDG_SESSION_CLASS_var();
        std::string XDG_CONFIG_DIRS = "XDG_CONFIG_DIRS=" + get_vars::get_XDG_CONFIG_DIRS_var();
        std::string XDG_SEAT = "XDG_SEAT=" + get_vars::get_XDG_SEAT_var();
        std::string XDG_RUNTIME_DIR = "XDG_RUNTIME_DIR=" + get_vars::get_XDG_RUNTIME_DIR_var();




        std::vector<char*> argv;
        for (const std::string& arg : args) {       // Build the command string
            argv.push_back(const_cast<char*>(arg.c_str()));      // Add each argument to the argv vector
        }
        argv.push_back(nullptr);  // The last element must be nullptr

        pid_t pid;

        // Get the pointer to the argv vector
        char* const* argvPtr = argv.data();

        char* const envp[] = {
            //const_cast<char*>("XDG_SESSION_PATH=/org/freedesktop/DisplayManager/Session0"),
            //const_cast<char*>("XCURSOR_SIZE=24"),
            //const_cast<char*>("XDG_SESSION_DESKTOP=KDE"),
            //const_cast<char*>("XDG_SESSION_TYPE=x11"),
            //const_cast<char*>("XDG_SEAT_PATH=/org/freedesktop/DisplayManager/Seat0"),
            // const_cast<char*>("XCURSOR_THEME=breeze_cursors"),
            // const_cast<char*>(LD_PRELOAD.c_str()),
            const_cast<char*>(DPCPP_HOME.c_str()),
            const_cast<char*>(DBUS_SESSION_BUS_ADDRESS.c_str()),
            const_cast<char*>(COLORTERM.c_str()),
            const_cast<char*>(TERM.c_str()),
            const_cast<char*>(XDG_CONFIG_DIRS.c_str()),
            const_cast<char*>(XDG_RUNTIME_DIR.c_str()),
            const_cast<char*>(XDG_SEAT.c_str()),
            const_cast<char*>(XDG_SESSION_CLASS.c_str()),
            const_cast<char*>(XDG_CURRENT_DESKTOP.c_str()),
            const_cast<char*>(XAUTHORITY.c_str()),
            const_cast<char*>(USER.c_str()),
            const_cast<char*>(DISPLAY.c_str()),
            const_cast<char*>(HOME.c_str()),
            const_cast<char*>(PATH.c_str()),
            nullptr
        };

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
void og(std::vector<std::string>& args) {
    std::vector<std::string> newArgs(args.begin() + 1, args.end());
    std::vector<char*> argv;
        for (const std::string& arg : newArgs) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
        argv.push_back(nullptr);  // Null-terminate the array
        pid_t pid = fork();                             // Fork a child process
        if (pid == -1) {                                // Forking failed
            perror("fork");
        } else if (pid == 0) {                          // This is the child process
            if (execvp(argv[0], argv.data()) == -1) {   // Execute the command
                if (errno == ENOENT) {
                std::cerr << "Shell: '" << args[0] << "' no such file or directory\n";
            } else {
                perror("execvp");
            }
            exit(EXIT_FAILURE);
            }
        } else {                                        // This is the parent process
            int status;
            if (waitpid(pid, &status, 0) == -1) {       // Wait for the child to finish
                perror("waitpid");
            }
        }
}
void run_from_current_dir(std::vector<std::string>& args) {
    size_t pos = args[0].find("./");
    if (pos != std::string::npos) {
        args[0].replace(pos, 2, get_vars::get_CURRENT_WORKING_DIRECTORY_var() + "/");
    }
}
