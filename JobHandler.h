#ifndef JOB_HANDLER_H
#define JOB_HANDLER_H

#include <iostream>
#include <signal.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <vector>



void startjob(const std::string& command, char *const argv[]);

void listJobs();

void terminateJob(size_t jobIndex);

void bringJobToForeground(size_t jobIndex);

void start_backround(std::vector<std::string>& args);

#endif // JOB_HANDLER_H
