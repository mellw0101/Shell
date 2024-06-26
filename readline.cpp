#include "readline.h"
#include "base_tools.h"

namespace fs = std::filesystem;

vector<string> history;
struct termios orig_termios;

void SimpleReadline::enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void SimpleReadline::disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

SimpleReadline::SimpleReadline()
{
    enableRawMode();
}

SimpleReadline::~SimpleReadline()
{
    disableRawMode();
}

string SimpleReadline::readLine()
{
    string line;
    char c;

    string red        = "\e[31m";
    string bold_green = "\e[1m\e[32m";
    string reset      = "\e[0m";
    string bold_white = "\e[1m\e[37m";

    string dir = fs::current_path().string();

    size_t pos = dir.find(get_vars::get_HOME_var());
    if (pos != string::npos)
    {
        dir.replace(pos, get_vars::get_HOME_var().length(), "~");
    }

    string USER = get_vars::get_USER::get_USER_var();
    string shell_no_color = "@(  )-> ";
    string shell = red + USER + reset + "@( " + bold_green + dir + reset + " )-> ";

    int length = shell_no_color.size() + USER.size() + dir.size() + 1;

    string prompt = shell;

    int cursorPos = 0;
    int historyIndex = history.size();
    cout << prompt;
    cout.flush();

    while (read(STDIN_FILENO, &c, 1) == 1 && c != '\n')
    {
        // functions
        if (c == '\033')
        {
            read(STDIN_FILENO, &c, 1);
            read(STDIN_FILENO, &c, 1);

            // Up and Down Arrows (History Navigation)
            if (c == 'A' || c == 'B')
            {
                // Up arrow
                if (c == 'A' && historyIndex > 0)
                {
                    if (!line.empty())
                    {
                        history[historyIndex] = line;
                    }
                    
                    historyIndex--;
                    cout << "\033[2K\r" << prompt << history[historyIndex];
                    line = history[historyIndex];
                    cursorPos = line.length();
                    cout.flush();
                    continue;
                }

                // Down arrow
                if (c == 'B' && historyIndex < history.size())
                {
                    history[historyIndex] = line;
                    historyIndex++;
                    std::cout << "\033[2K\r" << prompt << history[historyIndex];
                    line = history[historyIndex];
                    cursorPos = line.length();
                    std::cout.flush();
                    continue;
                }
                
                // Down arrow while line is empty will do nothing
                if (c == 'B' && line.empty())
                {
                    continue;
                }
                
                // Down arrow at last entry
                if (c == 'B' && historyIndex >= history.size())
                {
                    if (historyIndex == history.size())
                    {
                        history[historyIndex] = line;
                        historyIndex++;
                    }
                    
                    if (historyIndex - 1 == history.size())
                    {
                        history[historyIndex - 1] = line;
                    }
                    
                    std::cout << "\033[2K\r" << prompt;
                    line.clear();
                    cursorPos = line.length();
                    std::cout.flush();
                    continue;
                }

                continue;
            }

            // Right and Left Arrows (Cursor Movement)
            if (c == 'C' || c == 'D')
            {
                // Right arrow
                if (c == 'C')
                {
                    if (cursorPos < line.size())
                    {
                        cursorPos++;
                        std::cout << "\033[" << (0) << "C";
                        std::cout.flush();
                        continue;
                    }

                    continue;
                }

                // Left arrow
                if (c == 'D')
                {
                    if (cursorPos > 0)
                    {
                        cursorPos--;
                        std::cout << "\033[" << (0) << "D";
                        std::cout.flush();
                        continue;
                    }

                    continue;
                }

                continue;
            }

            // Ctrl+
            if (c == '1')
            {
                read(STDIN_FILENO, &c, 1); // read ';'
                read(STDIN_FILENO, &c, 1); // read '5'
                read(STDIN_FILENO, &c, 1); // read actual command (D for left, C for right)
            
                // Ctrl + Left
                if (c == 'D')
                {
                    while (cursorPos > 0 && line[cursorPos - 1] == ' ') cursorPos--;
                    while (cursorPos > 0 && line[cursorPos - 1] != ' ') cursorPos--;

                    cout << "\033[2K\r" << prompt << line;
                    cout << "\033[" << (cursorPos + length) << "G";
                    cout.flush();

                    continue;
                }

                // Ctrl + Right
                if (c == 'C')
                {
                    while (cursorPos < line.size() && line[cursorPos + 1] != ' ') cursorPos++;
                    while (cursorPos < line.size() && line[cursorPos + 1] == ' ') cursorPos++;

                    cout << "\033[2K\r" << prompt << line;
                    cout << "\033[" << (cursorPos + length) << "G";
                    cout.flush();

                    continue;
                }
            }

            continue;
        }

        // Backspace
        if (c == '\x7F')
        {
            if (cursorPos > 0)
            {
                line.erase(cursorPos - 1, 1);
                cursorPos--;
                
                cout << "\033[2K\r" << prompt << line;
                cout << "\033[" << (cursorPos + length) << "G";
                cout.flush();

                continue;
            }

            continue;
        }

        // if CTRL+D is pressed return exit
        if (c == 4)
        {
            return "exit";
        }

        // do nothing when these are pressed
        if (
            c == 1 ||   // CTRL+A
            c == 2 ||   // CTRL+B
            c == 5 ||   // CTRL+E
            c == 6 ||   // CTRL+F
            c == 7 ||   // CTRL+G
            c == 8 ||   // CTRL+H
            c == 9 ||   // CTRL+I
            c == 11 ||  // CTRL+K
            c == 12 ||  // CTRL+L
            c == 14 ||  // CTRL+N
            c == 15 ||  // CTRL+O
            c == 16 ||  // CTRL+P
            c == 18 ||  // CTRL+R
            c == 20 ||  // CTRL+T
            c == 21 ||  // CTRL+U
            c == 22 ||  // CTRL+V
            c == 23 ||  // CTRL+W
            c == 24 ||  // CTRL+X
            c == 25 ||  // CTRL+Y
            c == 29)    // CTRL+]
        {
            continue;
        }

        // append normal letters to line
        else
        {
            if (cursorPos >= 0)
            {
                line.insert(cursorPos, 1, c);
                cursorPos++;

                cout << "\033[2K\r" << prompt << line;
                cout << "\033[" << (cursorPos + length) << "G";
                cout.flush();

                continue;
            }
        }
    }
    
    cout << '\n';
    if (!line.empty())
    {
        history.push_back(line);
    }

    return line;
}

void SimpleReadline::loadHistoryFromFile(const string &filePath)
{
    ifstream historyFile(filePath);
    if (historyFile.is_open())
    {
        string line;
        while (getline(historyFile, line))
        {
            // use the history vector of your SimpleReadline object
            history.push_back(line);
        }

        historyFile.close();
    }
}

void SimpleReadline::appendHistoryToFile(const string &line, const string &filePath)
{
    // Open file in append mode
    ofstream historyFile(filePath, ios::app);
    if (historyFile.is_open())
    {
        // Append the line and a newline character to the file
        historyFile << line << '\n';
        historyFile.close();
    }
}
