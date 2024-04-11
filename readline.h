#ifndef READLINE_H
#define READLINE_H

#include <iostream>
#include <vector>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <filesystem>
#include <fstream>


class SimpleReadline {
private:
    std::vector<std::string> history;
    struct termios orig_termios;

    void enableRawMode();

    void disableRawMode();

public:
    SimpleReadline();

    ~SimpleReadline();

    std::string readLine();
    void loadHistoryFromFile(const std::string &filePath);
    void appendHistoryToFile(const std::string &line, const std::string &filePath);
};

#endif // READLINE_H

