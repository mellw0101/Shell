#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pty_master, pty_slave;
    char pty_name[100];

    // Open a new pseudo-terminal
    pty_master = posix_openpt(O_RDWR);
    grantpt(pty_master);
    unlockpt(pty_master);
    ptsname_r(pty_master, pty_name, sizeof(pty_name));
    pty_slave = open(pty_name, O_RDWR);

    pid_t pid = fork();
    if (pid == 0) {  // Child Process
        // Set up the terminal attributes
        struct termios term_attr;
        tcgetattr(pty_slave, &term_attr);
        term_attr.c_lflag = ECHO;
        tcsetattr(pty_slave, TCSANOW, &term_attr);

        // Bind stdio to the pseudo-terminal
        dup2(pty_slave, STDIN_FILENO);
        dup2(pty_slave, STDOUT_FILENO);
        dup2(pty_slave, STDERR_FILENO);

        // Execute the shell
        char* args[] = { (char*)"/bin/sh", nullptr };
        execvp("/bin/sh", args);
        exit(1);
    } else if (pid > 0) {  // Parent Process
        char buffer[256];
        ssize_t bytes;

        // Loop to forward terminal input/output
        while (true) {
            bytes = read(pty_master, buffer, sizeof(buffer));
            if (bytes <= 0) break;
            write(STDOUT_FILENO, buffer, bytes);

            bytes = read(STDIN_FILENO, buffer, sizeof(buffer));
            if (bytes <= 0) break;
            write(pty_master, buffer, bytes);
        }

        // Clean up
        close(pty_master);
        close(pty_slave);
        wait(nullptr);
    } else {
        std::cerr << "Fork failed\n";
        return 1;
    }

    return 0;
}
