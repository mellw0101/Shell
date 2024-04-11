#ifndef PIPE_H
#define PIPE_H

#include <iostream>
#include <ostream>
#include <regex>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <array>


namespace c_pipe {
    std::string executeCommandAndGetOutput(const std::string& command);
    std::string executeCommand(const std::string& command); // uses pipes to search for string in output of executed command
    std::string regex_escape(const std::string& input);
    bool search_string_all(std::string& targetstring, const std::string& stringToFind);
}

#endif // PIPE_H
