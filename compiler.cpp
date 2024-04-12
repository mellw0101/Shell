#include <chrono>
// #include <fstream>
#include <future>
#include <iostream>
#include <map>
// #include <sstream>
#include <string>
#include <thread>
#include <vector>

// sudo g++ -o /bin/cmks /usb/compiler.cpp
using namespace std;

map<int, promise<int>> promiseMap;

string get_HOME_var()
{
    const char* env_var = getenv("HOME");
    if (env_var == nullptr)
    {
        // Handle the case where PATH is not set or is empty
        return "";
    }
    return string(env_var);
}

void createStaticLibrary(const string &outputLibrary, const vector<string> &objectFiles)
{
    string command = "ar rcs " + outputLibrary;

    for (const string &objFile : objectFiles)
    {
        command += " " + objFile; // Add each object file to the command
    }

    int result = system(command.c_str());

    if (result == 0)
    {
        cout << "Static library '" << outputLibrary << "' created successfully." << endl;
    }
    else
    {
        cerr << "Error creating static library '" << outputLibrary << "'." << endl;
    }
}

void clang(const std::string& outputfile, const std::string& inputfile, const std::string& args, int threadId)
{
    auto startTime = std::chrono::high_resolution_clock::now(); // Record the start time
    std::string command = "clang++ " + args + " " + inputfile + " " + outputfile;
    std::cout << "Thread " << threadId << " is running.\n\n";
    int result = std::system(command.c_str());
    auto endTime = std::chrono::high_resolution_clock::now(); // Record the end time
    if (result == 0)
    {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        std::cout << "\n\x1b[32mThread " << threadId << " successfully made '" << outputfile << " in " << duration << " milliseconds.\x1b[0m\n\n";
    }
    else
    {
        std::cout << "\n\e[1m\e[31mERROR Thread " << threadId << " failed to make '" << outputfile << "'\n\n";
    }
    // Calculate the duration in milliseconds


    promiseMap[threadId].set_value(result);                         // Set the promise with the result
}

void link(const std::string& outputExecutable, const std::vector<std::string>& objectFiles, const std::string& args, int threadId, std::promise<int>& resultPromise)
{
    auto startTime = std::chrono::high_resolution_clock::now(); // Record the start time

    std::string command = "clang++";

    for (const std::string& objFile : objectFiles)
    {
        command += " " + objFile; // Add each object file to the command
    }

    command += " " + args; // Add additional arguments

    command += " -o " + outputExecutable;

    std::cout << "Thread " << threadId << " is running.\n";

    int result = std::system(command.c_str());

    auto endTime = std::chrono::high_resolution_clock::now(); // Record the end time

    if (result == 0)
    {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        std::cout << "Thread " << threadId << " successfully linked '" << outputExecutable << "' in " << duration << " milliseconds.\n";
    }
    else
    {
        std::cout << "Thread " << threadId << " failed to link '" << outputExecutable << "'\n";
    }

    resultPromise.set_value(result); // Set the promise with the result
}

std::string output_o(const std::string& name)
{
    return "-o /home/mellw/new_source/Shell-1/o.files/" + name + ".o";
}

std::string source_o(const std::string& name)
{
    return "-c /home/mellw/new_source/Shell-1/" + name + ".cpp";
}

struct args
{
    std::string o_args = "-O3 -funroll-loops -Rpass=loop-vectorize -flto -m64 -Wall -Werror -static -march=native -stdlib=libc++";
    std::string f_args = "-flto -s -static -L/usr/avr/13.2.0 -l:libgcc.a -march=skylake -mtune=skylake  -L/home/mellw/zlib -l:libz.a  -L/usr/lib -l:libc++.a -L/usr/lib -l:libc++abi.a ";
};

int make_o()
{
    args args;
    std::thread t1(clang, output_o("Shell"), source_o("Shell"), args.o_args, 1);
    std::thread t2(clang, output_o("c_cp"), source_o("c_cp"), args.o_args, 2);
    std::thread t3(clang, output_o("base_tools"), source_o("base_tools") , args.o_args, 3);
    std::thread t4(clang, output_o("math"), source_o("math"), args.o_args, 4);
    std::thread t5(clang, output_o("pipe"), source_o("pipe"), args.o_args, 5);
    std::thread t6(clang, output_o("c_ls"), source_o("c_ls"), args.o_args, 6);
    std::thread t7(clang, output_o("export_from_file"), source_o("export_from_file"), args.o_args, 7);
    std::thread t8(clang, output_o("string_manipulation"), source_o("string_manipulation"), args.o_args, 8);
    std::thread t9(clang, output_o("c_file"), source_o("c_file"), args.o_args, 9);
    std::thread t10(clang, output_o("run_wine"), source_o("run_wine"), args.o_args, 10);
    std::thread t11(clang, output_o("readline"), source_o("readline"), args.o_args, 11);
    std::thread t12(clang, output_o("JobHandler"), source_o("JobHandler"), args.o_args, 12);
    std::thread t13(clang, output_o("benchmarks"), source_o("benchmarks"), args.o_args, 13);
    std::thread t14(clang, output_o("c_gz"), source_o("c_gz"), args.o_args, 14);


    t1.join(); t2.join(); t3.join(); t4.join(); t5.join(); t6.join(); t7.join(); t8.join(); t9.join(); t10.join(); t11.join(); t12.join(); t13.join(); t14.join();

    // Wait for the results and collect them
    int result1 = promiseMap[1].get_future().get();
    int result2 = promiseMap[2].get_future().get();
    int result3 = promiseMap[3].get_future().get();
    int result4 = promiseMap[4].get_future().get();
    int result5 = promiseMap[5].get_future().get();
    int result6 = promiseMap[6].get_future().get();
    int result7 = promiseMap[7].get_future().get();
    int result8 = promiseMap[8].get_future().get();
    int result9 = promiseMap[9].get_future().get();
    int result10 = promiseMap[10].get_future().get();
    int result11 = promiseMap[11].get_future().get();
    int result12 = promiseMap[12].get_future().get();
    int result13 = promiseMap[13].get_future().get();
    int result14 = promiseMap[14].get_future().get();

    if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0 && result5 == 0 && result6 == 0 && result7 == 0 && result8 == 0 && result9 == 0 && result10 == 0 && result11 == 0 && result12 == 0 && result13 == 0 && result14 == 0)
    {
        std::cout << "\n\nDone turning cpp file(s) to .o\n\n"; return 0;
    }
    else
    {
        return 1;
    }
}

std::string o_input(const std::string& name)
{
    return "/home/mellw/new_source/Shell-1/o.files/" + name + ".o";
}

int make_exec()
{
    args args;
    std::vector<std::string> objectFiles = {
        o_input("Shell"),
        o_input("c_cp"),
        o_input("base_tools"),
        o_input("math"),
        o_input("pipe"),
        o_input("c_ls"),
        o_input("export_from_file"),
        o_input("string_manipulation"),
        o_input("c_file"),
        o_input("run_wine"),
        o_input("readline"),
        o_input("JobHandler"),
        o_input("benchmarks"),
        o_input("c_gz")
    };

    std::promise<int> resultPromise;
    std::future<int> resultFuture = resultPromise.get_future();

    std::thread t(link, "/home/mellw/new_source/Shell-1/Shell", objectFiles, args.f_args, 1, std::ref(resultPromise));
    t.join();

    int result = resultFuture.get(); // Wait for the result from the thread

    if (result == 0)
    {
        std::cout << "\e[32mDone linking object files to executable\e[0m" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "\e[31mLinking failed\e[0m" << std::endl;
        return 1;
    }
}

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        std::string o = "o";
        std::string e = "e";
        if (arg == o)
        {
            std::future<int> make_o_result = std::async(std::launch::async, make_o);
            int make_o= make_o_result.get();
            if (make_o == 0)
            {
                std::cout << "cmkS: make_o: compiled successfully.\n\n";
            }
            else
            {
                std::cerr << "cmkS: make_o: ERROR: failed to compile .o objects.\n";
                return 1;
            }
        }
        else if (arg == e)
        {
            std::future<int> make_exec_result = std::async(std::launch::async, make_exec);
            int make_exec = make_exec_result.get();
            if (make_exec == 0)
            {
                std::cout << "\nMain: make_exec completed successfully.\n\n";
            }
            else
            {
                std::cerr << "Main: make_exec failed.\n";
                return 1;
            }
        }
    }
    return 0;
}
