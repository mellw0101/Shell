#include "pipe.h"

namespace c_pipe {
    std::string executeCommandAndGetOutput(const std::string& command) {
        int pipefd[2];                                      // Create a pipe to capture the command's output
        if (pipe(pipefd) == -1) {
            perror("pipe");
            return "";
        }
        pid_t childPid = fork();
        if (childPid == -1) {
            perror("fork");
            return "";
        }
        if (childPid == 0) {                                // Child process
            close(pipefd[1]);                               // Close the write end of the pipe, as it's not needed in the child
            dup2(pipefd[0], STDIN_FILENO);                  // Redirect the standard output to the write end of the pipe
            close(pipefd[0]);
            const char* cmd = command.c_str();
            execlp(cmd, cmd, nullptr);                      // Execute the command
            perror("execlp");                               // If execlp fails, print an error message and exit
            exit(1);
        } else {                                            // Parent process
            close(pipefd[0]);                               // Close the read end of the pipe, as it's not needed in the parent
            constexpr size_t bufferSize = 4096;             // Create a buffer to read the command's output
            char buffer[bufferSize];
            std::string output = "";
            ssize_t bytesRead;
            while ((bytesRead = read(pipefd[1], buffer, bufferSize)) > 0) { // Read the command's output and capture it into the 'output' string
                output += std::string(buffer, bytesRead);
            }
            close(pipefd[1]);                               // Close the write end of the pipe
            int status;
            waitpid(childPid, &status, 0);                  // Wait for the child process to finish
            return output;
        }
    }
    std::string executeCommand(const std::string& command) {          // uses pipes to search for string in output of executed command
        std::string result; std::array<char, 128> buffer;

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            std::cerr << "Error executing the command." << std::endl;
            return "";
        }
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
        pclose(pipe); return result;
    }
    std::string regex_escape(const std::string& input) {
        // Characters to escape: \ ^ $ . | ? * + ( ) [ ] { }
        std::string specialChars = "\\^$.|?*+()[]{}";
        std::string escapedString;

        for (char c : input) {
            if (specialChars.find(c) != std::string::npos) {
                escapedString += '\\'; // Add a backslash to escape the character
            }
            escapedString += c;
        }

        return escapedString;
    }
    bool search_string_all(std::string& targetstring, const std::string& stringToFind) {
        // Create a regular expression pattern to match the string globally
        std::string escapedString = regex_escape(stringToFind);
        std::regex stringPattern("(" + escapedString + ")");

        std::vector<std::string> lines;
        size_t start = 0;


        // Split the target string into lines
        while (start < targetstring.length()) {
            size_t end = targetstring.find('\n', start);
            if (end == std::string::npos) {
                end = targetstring.length();
            }
            lines.push_back(targetstring.substr(start, end - start));
            start = end + 1;
        }

        bool found = false;

        // Process each line
        for (const std::string& line : lines) {
            // Search for the string in the line
            if (std::regex_search(line, stringPattern)) {
                // Print the line with highlighting
                std::string highlightedLine = std::regex_replace(line, stringPattern, "\033[1;31m$1\033[0m");
                std::cout << highlightedLine << std::endl;
                found = true;
            }
        }

        return found;
    }
}
