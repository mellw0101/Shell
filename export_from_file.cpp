#include "export_from_file.h"
#include "base_tools.h"
#include "c_cp.h"
#include "string_manipulation.h"

namespace fs = std::filesystem;

namespace tools {
    bool export_from_file(const std::string& FILEname, std::string& evar) {
        if (!fs::exists(FILEname)) {         // if target file does not exist
            error_message_no_halt("export_from_file", "target file '" + FILEname + "' does not exist");
            return false;
        }
        if (fs::is_directory(FILEname)) {    // if target file is a directory
            error_message_no_halt("export_from_file", "'" + FILEname + "' is a directory");
            return false;
        }
        if (fs::is_empty(FILEname)) {        // if target file is empty
            error_message_no_halt("export_from_file", "'" + FILEname + "' is empty");
            return false;
        }
        std::ifstream inFile;
        inFile.open(FILEname);
        if (!inFile.is_open()) {             // if function was unable to open the file
            error_message_no_halt("export_from_file", "Can't open '" + FILEname + "'");
            return false;
        }
        std::string var;
        std::stringstream buffer;
        while (getline(inFile, var)) {
            buffer << var << '\n';
        }
        evar = buffer.str();
        inFile.close();
        return true;
    }
    void eff_usage() {
        std::cout << "Usage:\n"
                     "\n"
                     "eff <filename> if in cwd or </full/path/to/file>\n"
                     "eff </full/path/to/file> <find> \"string to find\"\n"
                     "eff </full/path/to/file> <find> \"string to find\" <rw> \"replace with this\" <*> num of occurance to replace type all for all>\n"
                     "\n";
    }
    void use_c_cpp_syntax(std::string& targetstring) {
        string_manipulation::make_color::replaceSpacesAfterSlash(targetstring);
        string_manipulation::make_color::nums_outside_quotes(targetstring);
        string_manipulation::make_color::quotes_red(targetstring);
        string_manipulation::color_for_string_no_comments_or_quotes_bold(targetstring, "true", "\e[37m\e[1m");
        string_manipulation::color_for_string_no_comments_or_quotes_bold(targetstring, "false", "\e[37m\e[1m");
        string_manipulation::color_for_string_no_comments_or_quotes_bold(targetstring, "nullptr", "\e[37m\e[1m");
        string_manipulation::make_color::functions(targetstring);
        string_manipulation::make_color::green_char(targetstring);

        string_manipulation::make_color::reverse_replaceSpacesAfterSlash(targetstring);
    }
}
void eff(const std::vector<std::string>& args) {
    std::string content; content = "";                      // Defines content and then makes sure that content is an empty string
    if (args::find_arg(args, "-f") && args::find_arg(args, "-rw")) {
        std::string backup_file = "/tmp/tmp_backup";
        tools::rm_f(backup_file);
        if (!tools::export_from_file(args[1], content)) {
            return;
        }
        content.erase(content.find_last_not_of(" \t\n") + 1);
        tools::create_file(backup_file);
        c_cp(args[1], backup_file, true);
        if (!Checksum(args[1], backup_file)) {              // if checksums dosent match
            return;
        }
        if (!string_manipulation::eff_search_string(content, args::processArgs(args, "-f"))) {  // if find did not find a match
            return;
        }
        int num;
        if (!args::find_arg(args, args::processArgs_2(args, "-rw"))) {
            num = -1;
        }
        else {
            num = stringToInt(args::processArgs_2(args, "-rw"));
        }
        if (!string_manipulation::chose_replacement(content, args::processArgs(args, "-f"), args::processArgs(args, "-rw"), num)) {                            // eff cannot replace
            error_message_no_halt("eff_replace_string", "cannot replace '" + args::processArgs(args, "-f") + "' with '" + args::processArgs(args, "-rw") + "'");
            return;
        }
        if (tools::rm_f(args[1])) {
            tools::create_file(args[1]);
            tools::inputF(content, args[1]);
            tools::rm_f(backup_file);
        }
        return;
    }
    if (args::find_arg(args, "-f")) {
        if (!tools::export_from_file(args[1], content))
            return;
        string_manipulation::eff_search_string(content, args::processArgs(args, "-f"));
        return;
    }
    if (args.size() != 2) {
        tools::eff_usage();
        return;
    }
    if (!tools::export_from_file(args[1], content))
        return;
    if (check_extension(args[1], ".c") || check_extension(args[1], ".cpp"))
        tools::use_c_cpp_syntax(content);
    std::cout << "File content = '( \n\n" << content << "\n)'End of FILE!!!\n";
}
