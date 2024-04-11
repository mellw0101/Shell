#ifndef C_LS_H
#define C_LS_H

#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

void c_ls(const std::vector<std::string>& args);

#endif // C_LS_H
