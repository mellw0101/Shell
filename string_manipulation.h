#ifndef STRING_MANIPULATION_H
#define STRING_MANIPULATION_H

#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <unistd.h>

namespace string_manipulation   {
    bool eff_search_string(const std::string& targetstring, const std::string& wordToFind);
    bool eff_replace_string(std::string& targetstring, const std::string& wordToFind, const std::string& replacement);
    bool chose_replacement(std::string& targetstring, const std::string& wordToFind, const std::string& replacement, int occurrenceToReplace);
    void color_for_string_no_comments_or_quotes(std::string& targetstring, const std::string& search_string, const std::string& color);
    void color_for_string_no_comments_or_quotes_bold(std::string& targetstring, const std::string& search_string, const std::string& color);
    void color_simple(std::string& targetstring, const std::string& search_string, const std::string& color);
    namespace make_color {
        void green_char(std::string& targetstring);
        void blue_char(std::string& targetstring, const char c);
        void nums_outside_quotes(std::string& targetstring);
        void commentGray(std::string& targetstring);
        void quotes_red(std::string& targetstring);
        void string_blue(std::string& targetstring);
        void comments_gray(std::string& targetstring);
        void replaceSpacesAfterSlash(std::string& targetstring);
        void functions(std::string& targetstring);
        void green_char_test(std::string& targetstring);
        void reverse_replaceSpacesAfterSlash(std::string& targetstring);
    }
}

#endif // STRING_MANIPULATION_H
