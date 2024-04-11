#include <future>
#include <numeric>
#include <sched.h>
#include <set>
#include <string>
#include <sys/resource.h>
#include <unistd.h>
#include <csignal> // Required for signal handling
#include <unordered_map>

#include "JobHandler.h"
#include "base_tools.h"
#include "benchmarks.h"
#include "c_cp.h"
#include "c_file.h"
#include "c_gz.h"
#include "c_ls.h"
#include "export_from_file.h"
#include "math.h"
#include "pipe.h"
#include "readline.h"
#include "run.h"

std::vector<std::string> commandHistory;                    // Global variables for command history
int historyIndex = -1;
volatile sig_atomic_t ctrlCPressed = 0;

void ctrlCHandler(int signalNumber) {
    ctrlCPressed = 1;
}
enum Command {
    CF,
    EFF,
    ITF,
    RUN,
    TEST,
    OG,
    CD,
    CP,
    LS,
    MKDIR,
    MATH,
    CFILE,
    WINE,
    BACKGROUND,
    JOBS,
    KILLJOB,
    BF,
    LF,
    TERMSIZE,
    MEM,
    PRIMESIVE,
    GZ,
    MV,
    OTHER
};
Command stringToEnum(const std::string& command) {
    static const std::unordered_map<std::string, Command> commandMap = {
        {"cf", CF},
        {"eff", EFF},
        {"itf", ITF},
        {"run", RUN},
        {"test", TEST},
        {"og", OG},
        {"cd", CD},
        {"cp", CP},
        {"ls", LS},
        {"mkdir", MKDIR},
        {"math", MATH},
        {"file", CFILE},
        {"wine", WINE},
        {"&", BACKGROUND},
        {"jobs", JOBS},
        {"killjob", KILLJOB},
        {"bf", BF},
        {"lf", LF},
        {"ts", TERMSIZE},
        {"mem_test", MEM},
        {"prime_sive", PRIMESIVE},
        {"gz", GZ},
        {"mv", MV}
    };

    auto it = commandMap.find(command);
    if (it != commandMap.end()) {
        return it->second;
    }
    return OTHER;
}
void executeCommand(const std::vector<std::string> &raw_args) {
    std::vector<std::string> args = args::combineArgsBetweenQuotes(raw_args);
    Command commandEnum = OTHER;

    if (args.back() == "&") {
        commandEnum = BACKGROUND;
    } else {
        commandEnum = stringToEnum(args[0]);
    }
    switch (commandEnum) {
        case CF:    // create_file
            cf(args);
            break;
        case MATH:
            math(args);
            break;
        case EFF:
            eff(args);
            break;
        case CD:
            cd(args);
            break;
        case LS:
            c_ls(args);
            break;
        case ITF:
            itf(args);
            break;
        case MKDIR:
            cmkdir(args);
            break;
        case CFILE:
            c_file(args);
            break;
        case CP:
            cp(args);
            break;
        case TERMSIZE:
            termsize();
            break;
        case BACKGROUND:
            start_backround(args);
            break;
        case KILLJOB:
            terminateJob(stringToInt(args[1]) - 1);
            break;
        case JOBS:
            listJobs();
            break;
        case BF:
            bringJobToForeground(stringToInt(args[1]) - 1);
            break;
        case LF:
            listFiles(args[1]);
            break;
        case OG:
            og(args);
            break;
        case MEM:
            mem_read();
            break;
        case PRIMESIVE:
            prime_sive(stringToInt(args[1]));
            break;
        case GZ:
            gz(args);
            break;
        case MV:
            mv(args);
            break;
        case OTHER:
        default:
            std::string binaryPath = search_in_PATH(args[0]);
            try {
                auto asyncTask = runBinaryAsync_with_env_wargs(binaryPath, args);
                asyncTask.get(); // Wait for the async task to finish
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;


                std::vector<char*> argv;
                for (const std::string& arg : args) {
                    argv.push_back(const_cast<char*>(arg.c_str()));
                }
                argv.push_back(nullptr);  // Null-terminate the array

                // Fork a child process
                pid_t pid = fork();

                if (pid == -1) {
                    // Forking failed
                    perror("fork");
                } else if (pid == 0) {
                    // This is the child process
                    // Execute the command
                    if (execvp(argv[0], argv.data()) == -1) {
                        if (errno == ENOENT) {
                        std::cerr << "Shell: '" << args[0] << "' no such file or directory\n";
                    } else {
                        perror("execvp");
                    }
                    exit(EXIT_FAILURE);
                    }
                } else {
                    // This is the parent process
                    // Wait for the child to finish
                    int status;
                    if (waitpid(pid, &status, 0) == -1) {
                        perror("waitpid");
                    }
                }
            }
            break;
    }
}

int main() {
    signal(SIGINT, ctrlCHandler);                           // Register the signal handler
    setenv("PATH", get_vars::get_PATH_var().c_str(), 1);
    SimpleReadline sr;
    sr.loadHistoryFromFile(get_vars::get_HOME_var() + "/.ShellHistory");
    std::string userInput;
    while (true) {                                          // main shell loop
        if (ctrlCPressed) {                                 // Check the flag to see if Ctrl+C was pressed
            std::cout << "\n";
            ctrlCPressed = 0;                               // Reset the flag
            continue;                                       // Skip to the next iteration
        }
        userInput = sr.readLine();
        if (userInput == "exit") {                          // User pressed Ctrl+D or enters exit as a command
            break;
        } else {
            if (!userInput.empty())
                sr.appendHistoryToFile(userInput , get_vars::get_HOME_var() + "/.ShellHistory");
            if (userInput.find('|') != std::string::npos) { // Check if the input contains a pipe character '|'
                std::vector<std::string> commands;
                std::istringstream inputStream(userInput);
                std::string command;
                while (std::getline(inputStream, command, '|')) {// Trim leading and trailing whitespace from each command
                    size_t first = command.find_first_not_of(" \t\n");
                    size_t last = command.find_last_not_of(" \t\n");
                    if (first != std::string::npos && last != std::string::npos) {
                        commands.push_back(command.substr(first, last - first + 1));
                    }
                }
                std::stringstream com;
                for (const std::string& cmd : commands) com << cmd << " ";
                std::vector<std::string> args;
                std::istringstream iss(com.str());
                std::string token;
                while (iss >> token) args.push_back(token);

                if (args.size() == 3) {
                    // running first command in pipe and saving output to string output
                    std::string output = c_pipe::executeCommand(args[0]);
                    if (args[1] == "find") c_pipe::search_string_all(output, args[2]);
                    else error_message_no_halt("Usage: pipes", "<command> | <find> <letter or string to find");
                }
                else if (args.size() == 4) {
                    std::string output = c_pipe::executeCommand(args[0] + " " + args[1]);
                    if (args[2] == "find") c_pipe::search_string_all(output, args[3]);
                    else error_message("usage pipes", "<command> | <find> <letter or string to find");
                }
            } else {                                        // No pipe detected, split the input into tokens
                std::vector<std::string> args;
                std::istringstream iss(userInput);
                std::string token;
                while (iss >> token)
                    args.push_back(token);
                if (!args.empty())                          // Execute the command if args are not empty
                    executeCommand(args);
            }
        }
    }
    return 0;
}
