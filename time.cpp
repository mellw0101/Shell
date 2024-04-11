#include "c_to_cpp.h"
#include <string>

// Constants for exit status codes
// constexpr int __EXIT_SUCCESS = 0;
// constexpr int __EXIT_FAILURE = 1;
// constexpr int EXIT_CANCELED = 2;
constexpr int EXIT_ENOENT = 3;
// constexpr int SIGNALLED_OFFSET = 128;

const char* PROGRAM_NAME = "time"; // Moved PROGRAM_NAME to global scope

// Struct to hold resource usage
struct ResourceUsage {
    struct timeval elapsed;
    struct rusage ru;
    int waitstatus;
};

// Function to print usage information
void usage(int status) {
    if (status != EXIT_SUCCESS) {
        std::cerr << "Try '" << PROGRAM_NAME << " --help' for more information." << std::endl;
        exit(status);
    }

    std::cout <<
        "Usage: " << PROGRAM_NAME << " [OPTIONS] COMMAND [ARG]...\n"
        "Run COMMAND, then print system resource usage.\n\n"
        "  -a, --append              with -o FILE, append instead of overwriting\n"
        "  -f, --format=FORMAT       use the specified FORMAT instead of the default\n"
        "  -o, --output=FILE         write to FILE instead of STDERR\n"
        "  -p, --portability         print POSIX standard 1003.2 conformant string:\n"
        "                            real %%e\n"
        "                            user %%U\n"
        "                            sys %%S\n"
        "  -q, --quiet               do not print information about abnormal program\n"
        "                            termination (non-zero exit codes or signals)\n"
        "  -v, --verbose             print all resource usage information instead of\n"
        "                            the default format\n"
        "  -h,  --help               display this help and exit\n"
        "  -V,  --version            output version information and exit\n"
        "\n"
        "Commonly used format sequences for -f/--format:\n"
        "  %%   a literal '%'\n"
        "  %C   command line and arguments\n"
        // ... (other format sequences)
        << std::endl;

    // ...
    exit(EXIT_SUCCESS);
}

// Function to run a command and get resource usage
void runCommand(const std::vector<std::string>& cmd, ResourceUsage& res) {
    pid_t pid;
    sigset_t mask;
    sigset_t omask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mask, &omask);

    res.elapsed = timeval{};
    res.ru = rusage{};
    res.waitstatus = 0;

    struct timeval start, end;
    gettimeofday(&start, nullptr);

    pid = fork();
    if (pid < 0) {
        std::cerr << "Error: cannot fork" << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        char** args = new char*[cmd.size() + 1];
        for (size_t i = 0; i < cmd.size(); ++i) {
            args[i] = const_cast<char*>(cmd[i].c_str());
        }
        args[cmd.size()] = nullptr;

        execvp(args[0], args);
        std::cerr << "Error: cannot run " << args[0] << std::endl;
        delete[] args;
        _exit(EXIT_ENOENT);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        gettimeofday(&end, nullptr);
        timersub(&end, &start, &res.elapsed);

        if (WIFSTOPPED(status)) {
            std::cerr << "Command stopped by signal " << WSTOPSIG(status) << std::endl;
            res.waitstatus = status;
        } else if (WIFSIGNALED(status)) {
            std::cerr << "Command terminated by signal " << WTERMSIG(status) << std::endl;
            res.waitstatus = status;
        } else if (WIFEXITED(status)) {
            res.waitstatus = status;
        } else {
            std::cerr << "Unknown status from command (" << status << ")" << std::endl;
            exit(EXIT_FAILURE);
        }

        getrusage(RUSAGE_CHILDREN, &res.ru);

        sigprocmask(SIG_SETMASK, &omask, nullptr);
    }
}

int time(int argc, char* argv[]) {
    // Command line arguments
    bool verbose = true;
    const char* outfile = nullptr;
    FILE* outfp = stderr;
    bool append = false;
    const char* output_format = "%E %U %S";  // Define your default format

    // Parse command line options
    int optc;
    while ((optc = getopt(argc, argv, "af:o:pqvV")) != -1) {
        switch (optc) {
            case 'a':
                append = true;
                break;
            case 'f':
                output_format = optarg;
                break;
            case 'o':
                outfile = optarg;
                break;
            case 'p':
                // output_format = "YOUR_POSIX_FORMAT_HERE";  // Define your POSIX format
                break;
            case 'q':
                // Handle quiet mode
                break;
            case 'v':
                verbose = true;
                break;
            case 'V':
                std::cout << "YOUR_VERSION_STRING_HERE" << std::endl;  // Define your version string
                exit(EXIT_SUCCESS);
            default:
                usage(EXIT_FAILURE);
        }
    }

    // Remaining arguments are the command and its arguments
    std::vector<std::string> cmd;
    for (int i = optind; i < argc; ++i) {
        cmd.emplace_back(argv[i]);
    }

    if (cmd.empty()) {
        std::cerr << "Error: missing command to execute." << std::endl;
        usage(EXIT_FAILURE);
    }

    // Open the output file if specified
    if (outfile != nullptr) {
        if (append) {
            outfp = fopen(outfile, "a");
        } else {
            outfp = fopen(outfile, "w");
        }

        if (outfp == nullptr) {
            std::cerr << "Error: cannot open output file '" << outfile << "' for writing." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Run the command and measure resource usage
    ResourceUsage res;
    runCommand(cmd, res);


    // Close the output file if it's not stderr
    if (outfp != stderr) {
        fclose(outfp);
    }

    return res.waitstatus;
}
