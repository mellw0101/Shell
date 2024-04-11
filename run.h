#ifndef RUN_H
#define RUN_H

#include <cstring>
#include <spawn.h>
#include <sys/wait.h>
#include <string>
#include <future>
#include <vector>

std::future<void> wine(const std::vector<std::string>& args);
std::future<void> runBinaryAsync_with_env(const std::string& binaryPath);
std::future<void> runBinaryAsync_with_env_wargs(const std::string& binaryPath, const std::vector<std::string>& args);
void og(std::vector<std::string>& args);

#endif // RUN_H
