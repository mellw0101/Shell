#ifndef READLINE_H
#define READLINE_H

#include <iostream>
#include <vector>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <filesystem>
#include <fstream>

using namespace std;

class SimpleReadline
{
private:
    vector<string> history;
    struct termios orig_termios;

    void enableRawMode();

    void disableRawMode();

public:
    SimpleReadline();

    ~SimpleReadline();

    std::string readLine();
    void loadHistoryFromFile(const string &filePath);
    void appendHistoryToFile(const string &line, const string &filePath);
};

#endif // READLINE_H

