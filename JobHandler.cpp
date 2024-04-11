#include "JobHandler.h"
#include "base_tools.h"

struct Job {
    pid_t pid;
    std::string command;
};

std::vector<Job> jobs;

void startjob(const std::string& command, char *const argv[]) {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {  // Child process
        setsid();
        execvp(command.c_str(), argv);
        std::cerr << "execvp failed" << std::endl;
        exit(EXIT_FAILURE);
    } else {  // Parent process
        jobs.push_back({pid, command});
        std::cout << "Job (" << command << ") started: (PID: " << pid << ")" << std::endl;
    }
}

void listJobs() {
    for (size_t i = 0; i < jobs.size(); ++i) {
        std::cout << "Job " << i + 1 << ": " << jobs[i].command << " (PID: " << jobs[i].pid << ")" << std::endl;
    }
}

void terminateJob(size_t jobIndex) {
    if (jobIndex < jobs.size()) {
        kill(jobs[jobIndex].pid, SIGTERM);
        jobs.erase(jobs.begin() + jobIndex);
    } else {
        std::cerr << "Invalid job index" << std::endl;
    }
}

void bringJobToForeground(size_t jobIndex) {
    if (jobIndex >= jobs.size()) {
        std::cerr << "Invalid job index" << std::endl;
        return;
    }

    pid_t pid = jobs[jobIndex].pid;
    int status;
    tcsetpgrp(STDIN_FILENO, getpgid(pid));  // Set the foreground process group of the terminal

    if (kill(-pid, SIGCONT) < 0) {  // Send SIGCONT to the process group to continue the job if it's stopped
        perror("kill (SIGCONT)");
    }

    // Wait for the process to change state; this will block until the process exits or stops
    waitpid(pid, &status, WUNTRACED);

    tcsetpgrp(STDIN_FILENO, getpgid(getpid()));  // Set the foreground process group of the terminal back to the shell
}

void start_backround(std::vector<std::string>& args) {
    args.pop_back();
    std::string command = search_in_PATH(args[0]);
    char** argv = vectorToArgv(args);
    if (command == "Command not found in any PATH directory.") {
        return;
    }
    startjob(args[0].c_str() , argv);
    cleanUpArgv(argv);
}

