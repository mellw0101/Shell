#include "string_manipulation.h"
#include "base_tools.h"

namespace string_manipulation   {
    bool eff_search_string_old(std::string& targetstring, const std::string& wordToFind) {
        if (wordToFind == "err1") {
            std::cerr << "Error: find requires an argument." << std::endl;
            return false;
        }
        // Create a regular expression pattern to match the whole word
        std::regex wordPattern("\\b" + wordToFind + "\\b");

        // Search for the whole word in the string
        std::smatch match;
        if (std::regex_search(targetstring, match, wordPattern)) {
            std::cout << "eff_search_string: string '" << wordToFind << "' found at position " << match.position() << std::endl;
            return true;
        } else {
            std::cout << "eff_search_string: Word '" << wordToFind << "' not found" << std::endl;
            return false;
        }
    }
    bool eff_search_string(const std::string& targetstring, const std::string& wordToFind) {
        if (wordToFind.empty()) {
            std::cerr << "Error: find requires a non-empty argument." << std::endl;
            return false;
        }

        // Create a regular expression pattern to match the whole word
        std::regex wordPattern("\\b" + wordToFind + "\\b");

        // Initialize a stringstream to extract lines
        std::stringstream ss(targetstring);
        std::string line;
        int lineNum = 1;
        bool found = false;

        // Loop through lines of the input string
        while (std::getline(ss, line)) {
            std::smatch match;
            std::string::const_iterator searchStart(line.cbegin());

            // Search for the whole word in the line
            while (std::regex_search(searchStart, line.cend(), match, wordPattern)) {
                found = true;
                std::cout << "eff_search_string: '" << wordToFind << "' \033[32mfound\033[0m at Line " << lineNum << ":" << match.position() << std::endl;
                searchStart = match.suffix().first;
            }
            lineNum++;
        }
        if (!found)
            error_message_no_halt("eff_search_string", "'" + wordToFind + "' not found");
        return found;
    }
    bool eff_replace_string(std::string& targetstring, const std::string& wordToFind, const std::string& replacement) {
        // Create a regular expression pattern to match the whole word
        std::regex wordPattern("\\b" + wordToFind + "\\b");

        // Replace the word in the target string
        targetstring = std::regex_replace(targetstring, wordPattern, replacement);

        // Check if any replacement was made
        return targetstring != wordToFind; // Returns true if a replacement occurred
    }
    bool eff_replace_string_all_or_first(std::string& targetstring, const std::string& wordToFind, const std::string& replacement, bool replaceAll = true) {
        if (wordToFind.empty()) {
            std::cerr << "Error: wordToFind cannot be empty." << std::endl;
            return false;
        }

        // Create a regular expression pattern to match the whole word
        std::regex wordPattern("\\b" + wordToFind + "\\b");

        // Replace the word in the target string
        if (replaceAll) {
            // Replace all occurrences
            targetstring = std::regex_replace(targetstring, wordPattern, replacement);
        } else {
            // Replace only the first occurrence
            targetstring = std::regex_replace(targetstring, wordPattern, replacement, std::regex_constants::format_first_only);
        }

        // Check if any replacement was made
        return targetstring.find(wordToFind) != std::string::npos;
    }
    int eff_replace_string_pick_num(std::string& targetstring, const std::string& wordToFind, const std::string& replacement, int occurrence = -1) {
        if (wordToFind.empty()) {
            std::cerr << "Error: wordToFind cannot be empty." << std::endl;
            return 0; // Return 0 to indicate no replacements were made
        }

        // Create a regular expression pattern to match the whole word
        std::regex wordPattern("\\b" + wordToFind + "\\b");

        // If occurrence is -1, replace all occurrences
        if (occurrence == -1) {
            targetstring = std::regex_replace(targetstring, wordPattern, replacement);
            return -1; // Return -1 to indicate all occurrences were replaced
        } else if (occurrence >= 1) {
            std::sregex_iterator it(targetstring.begin(), targetstring.end(), wordPattern);
            std::sregex_iterator end;

            int i = 1;
            while (it != end && i < occurrence) {
                ++it;
                ++i;
            }

            if (it != end) {
                std::smatch match = *it;
                targetstring.replace(match.position(), match.length(), replacement);
                return occurrence; // Return the occurrence number to indicate the replacement was made
            }
        }

        error_message_no_halt("eff_replace_string", "occurrence should be all or a positive integer");
        return 0; // Return 0 to indicate no replacements were made
    }
    bool chose_replacement(std::string& targetstring, const std::string& wordToFind, const std::string& replacement, int occurrenceToReplace) {
        int replacedOccurrences = eff_replace_string_pick_num(targetstring, wordToFind, replacement, occurrenceToReplace);

        if (replacedOccurrences == 0) {
            std::cout << "eff_replace_string: No replacements made\n";
            return false;
        } else if (replacedOccurrences == -1) {
            std::cout << "eff_replace_string: Replaced all occurrences\n";
            return true;
        } else {
            std::cout << "eff_replace_string: Replaced occurrence '" << occurrenceToReplace << "' \n";
            return true;
        }
        return false;
    }
    bool eff_replace_char(std::string& targetstring, const char charToFind, const std::string& replacement) {
        size_t foundPos = targetstring.find(charToFind);

        if (foundPos != std::string::npos) {
            // Replace the character in the target string with the replacement string
            targetstring.replace(foundPos, 1, replacement);
            return true; // A replacement occurred
        }

        return false; // No replacement was made
    }
    void insertStringAtPosition(std::string& original, const std::string& toInsert, int targetLine, int targetPos) {
        std::string result;
        int lineCount = 1;
        int currentPos = 0;

        for (char character : original) {
            if (lineCount == targetLine && currentPos == targetPos) {
                result += toInsert;  // Insert the string at the specified position
            }

            if (character == '\n') {
                lineCount++;
                currentPos = 0;
            } else {
                currentPos++;
            }

            result += character;
        }

        original = result;
    }
    void color_for_string_no_comments_or_quotes(std::string& targetstring, const std::string& search_string, const std::string& color){
        std::string regex_pattern = "\\b"+search_string+"\\b(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)";
        std::regex string(regex_pattern);
        // bool inside_quotes = false;
        std::string word;
        std::istringstream iss(targetstring);
        std::string line;
        int lineNum = 1;
        int length = 0;
        int last_line_num = 0;

        while (std::getline(iss, line)) {               // Loop through lines of the input string
            std::smatch match;
            std::string::const_iterator searchStart(line.cbegin());
            while (std::regex_search(searchStart, line.cend(), match, string)) {// Search for the whole word in the line
                if (last_line_num != lineNum){
                    length = 0;
                } else {
                    length = length + 9;
                }
                length = length + match.position() + 1;
                // std::cout << "start :quote :" << quote_num << " line :" << lineNum << ":" << length << ":" << length + match.length() << "\n";
                insertStringAtPosition(targetstring, color, lineNum, length - 1);
                insertStringAtPosition(targetstring, "\e[0m", lineNum, length + match.length() + 4);
                length = length + 1;
                searchStart = match.suffix().first;
                last_line_num = lineNum;
                length = length + match.length() - 2;
            }
            lineNum++;
        }
        // while (iss >> word){
        //     if (word.find('"') != std::string::npos && !inside_quotes){
        //         inside_quotes = true;
        //     } else if (word.find('"') != std::string::npos && !inside_quotes){
        //         inside_quotes = false;
        //     } else if (!inside_quotes && word == "string"){
        //         targetstring = std::regex_replace(targetstring, string, "\e[94m$&\e[0m");
        //     }
        // }
    }
    void color_for_string_no_comments_or_quotes_bold(std::string& targetstring, const std::string& search_string, const std::string& color){
        std::string regex_pattern = "\\b"+search_string+"\\b(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)";
        std::regex string(regex_pattern);
        // bool inside_quotes = false;
        std::string word;
        std::istringstream iss(targetstring);
        std::string line;
        int lineNum = 1;
        int length = 0;
        int last_line_num = 0;

        while (std::getline(iss, line)) {               // Loop through lines of the input string
            std::smatch match;
            std::string::const_iterator searchStart(line.cbegin());
            while (std::regex_search(searchStart, line.cend(), match, string)) {// Search for the whole word in the line
                if (last_line_num != lineNum){
                    length = 0;
                } else {
                    length = length + 9;
                }
                length = length + match.position() + 1;
                // std::cout << "start :quote :" << quote_num << " line :" << lineNum << ":" << length << ":" << length + match.length() << "\n";
                insertStringAtPosition(targetstring, color, lineNum, length - 1);
                insertStringAtPosition(targetstring, "\e[0m", lineNum, length + match.length() + 8);
                length = length + 1;
                searchStart = match.suffix().first;
                last_line_num = lineNum;
                length = length + match.length() - 2;
            }
            lineNum++;
        }
        // while (iss >> word){
        //     if (word.find('"') != std::string::npos && !inside_quotes){
        //         inside_quotes = true;
        //     } else if (word.find('"') != std::string::npos && !inside_quotes){
        //         inside_quotes = false;
        //     } else if (!inside_quotes && word == "string"){
        //         targetstring = std::regex_replace(targetstring, string, "\e[94m$&\e[0m");
        //     }
        // }
    }
    void color_simple(std::string& targetstring, const std::string& search_string, const std::string& color){
        std::regex regex_pattern("\\b"+search_string+"\\b(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)");
        std::regex pattern(regex_pattern);
        std::string result = std::regex_replace(targetstring, pattern, color+search_string+"\e[0m");
    }
    namespace make_color {
        void green_char(std::string& targetstring) {

            bool inside_quotes = false;
            bool inside_single_quotes = false;
            bool is_std = false;
            bool afterDoubleSlash = false;
            bool found = false;
            bool include_found = false;
            bool special_comment = false;
            bool yellow_found = false;
            bool return_value_found = false;

            std::string result;  // Initialize an empty result string

            for (size_t i = 0; i < targetstring.length(); ++i) {
                if  // start of quote
                    ( targetstring[i] == '"' )
                        {
                            // quote found and not already inside quote
                            if (!inside_quotes) {
                                inside_quotes = true;
                            }
                            // last quote found
                            else {
                                result += "\e[31m";
                                // Exiting a quoted section
                                inside_quotes = false;
                            }
                        }
                if  // start of single quote
                    (
                        targetstring[i] == '\'' &&
                        !afterDoubleSlash &&
                        !inside_quotes &&
                        !special_comment
                    )
                        {
                            if (!inside_single_quotes) {
                                // Entering a quoted section
                                inside_single_quotes = true;
                                result += "\e[94m";
                            } else {
                                // Exiting a quoted section
                                inside_single_quotes = false;
                            }
                        }
                if  // end of single quote
                    (
                        targetstring[i - 1] == '\'' &&
                        targetstring[  i  ] != '"' &&
                        !inside_single_quotes &&
                        !afterDoubleSlash &&
                        !inside_quotes &&
                        !special_comment
                    )
                        {
                            result += "\e[0m";
                        }
                if  // comment is found
                    (
                        targetstring[  i  ] == '/' && i + 1 < targetstring.length() &&
                        targetstring[i + 1] == '/'
                    )
                        {
                            result += "\e[90m";
                            afterDoubleSlash = true;
                        }
                if  // start of special comment found
                    (
                        targetstring[  i  ] == '/' &&
                        targetstring[i + 1] == '*'
                    )
                        {
                            result += "\e[90m";
                            special_comment = true;
                        }
                if  // end of special comment found
                    (
                        targetstring[i - 1] == '/' &&
                        targetstring[i - 2] == '*'
                    )
                        {
                            result += "\e[0m";
                            special_comment = false;
                        }
                if  // include is found
                    ( targetstring[i] == '#' )
                        {
                            include_found = true;
                            result += "\e[32m";
                        }
                if  // include has no headerfile reset color
                    ( include_found )
                        {
                            if  // whitespace or newline is found without ' < ' or ' " ' reset color
                                (
                                    (
                                        targetstring[  i  ] == '\n' ||
                                        targetstring[  i  ] == '('
                                    )
                                    &&
                                    (
                                        targetstring[i + 1] != '<' ||
                                        targetstring[i + 1] != '"'
                                    )
                                )
                                    {
                                        result += "\e[0m";
                                        include_found = false;
                                    }
                        }
                if  // std is found and not a comment
                    (
                        (
                            targetstring[  i  ] == 's' && i + 1 < targetstring.length() &&
                            targetstring[i + 1] == 't' &&
                            targetstring[i + 2] == 'd'
                        )
                        &&
                        (
                            !afterDoubleSlash &&
                            !include_found
                        )
                    )
                        {
                            result += "\e[37m\e[1m";
                            is_std = true;
                        }
                if  // last letter was a green char
                    (found)
                        {
                        // if another char is found after do not reset color
                        if (targetstring[i] == '*' || targetstring[i] == ':' || targetstring[i] == '|' || targetstring[i] == '-' || targetstring[i] == ',' || targetstring[i] == '+' || targetstring[i] == '{' || targetstring[i] == '}' || targetstring[i] == '=' || targetstring[i] == '('|| targetstring[i] == ')' || targetstring[i] == ';' || targetstring[i] == '<' || targetstring[i] == '>' || targetstring[i] == '!' || targetstring[i] == '&'){

                        }
                        // else reset color
                        else {
                            result += "\e[0m";
                            found = false;
                        }
                        }
                if  // start of char to color green
                    (
                        (
                            targetstring[i] == '*' ||
                            targetstring[i] == ':' ||
                            targetstring[i] == '|' ||
                            targetstring[i] == '-' ||
                            targetstring[i] == ',' ||
                            targetstring[i] == '+' ||
                            targetstring[i] == '{' ||
                            targetstring[i] == '}' ||
                            targetstring[i] == '=' ||
                            targetstring[i] == '(' ||
                            targetstring[i] == ')' ||
                            targetstring[i] == ';' ||
                            targetstring[i] == '<' ||
                            targetstring[i] == '>' ||
                            targetstring[i] == '!' ||
                            targetstring[i] == '&'
                        )
                        &&
                        (
                            !inside_quotes &&
                            !inside_single_quotes &&
                            !afterDoubleSlash &&
                            !is_std &&
                            !include_found &&
                            !special_comment
                        )
                    )
                        {
                            result += "\e[0m\e[32m";
                            found = true;
                        }
                if  // if letter is comment do nothing to it
                    ( afterDoubleSlash )
                        {

                            if  // letter is newline reset color
                                (
                                    targetstring[i] == '\n'
                                )
                                    {
                                        result += "\e[0m";
                                        afterDoubleSlash = false;
                                    }
                        }

                if  // std has been found
                    ( is_std )
                        {
                        if  /* double colon is found make non bold and gray */
                            (
                                targetstring[i - 1] != ':' &&
                                targetstring[  i  ] == ':'
                            )
                                {
                                    result += "\e[0m\e[90m";
                                }
                        if  /* at last colon do nothing */
                            (
                                targetstring[  i  ] == ':' &&
                                targetstring[i - 1] == ':'
                            )
                                {
                                }
                        if  // string is found
                            (
                                (
                                    targetstring[  i  ] == 's' && i + 1 < targetstring.length() &&
                                    targetstring[i + 1] == 't' &&
                                    targetstring[i + 2] == 'r' &&
                                    targetstring[i + 3] == 'i' &&
                                    targetstring[i + 4] == 'n' &&
                                    targetstring[i + 5] == 'g'
                                )
                                &&
                                (
                                    !afterDoubleSlash
                                )
                            )
                                {
                                    result += "\e[34m";
                                }
                        if  // vector is found
                            (
                                (
                                    targetstring[  i  ] == 'v' && i + 1 < targetstring.length() &&
                                    targetstring[i + 1] == 'e' &&
                                    targetstring[i + 2] == 'c' &&
                                    targetstring[i + 3] == 't' &&
                                    targetstring[i + 4] == 'o' &&
                                    targetstring[i + 5] == 'r'
                                )
                                &&
                                (
                                    !afterDoubleSlash
                                )
                            )
                                {
                                    result += "\e[34m";
                                }
                        if  // future is found
                            (
                                (
                                    targetstring[  i  ] == 'f' && i + 1 < targetstring.length() &&
                                    targetstring[i + 1] == 'u' &&
                                    targetstring[i + 2] == 't' &&
                                    targetstring[i + 3] == 'u' &&
                                    targetstring[i + 4] == 'r' &&
                                    targetstring[i + 5] == 'e'
                                )
                                &&
                                (
                                    !afterDoubleSlash
                                )
                            )
                                {
                                    result += "\e[34m";
                                }
                        if  // istringstream is found
                            (
                                (
                                    targetstring[  i   ] == 'i' && i + 1 < targetstring.length() &&
                                    targetstring[i +  1] == 's' &&
                                    targetstring[i +  2] == 't' &&
                                    targetstring[i +  3] == 'r' &&
                                    targetstring[i +  4] == 'i' &&
                                    targetstring[i +  5] == 'n' &&
                                    targetstring[i +  6] == 'g' &&
                                    targetstring[i +  7] == 's' &&
                                    targetstring[i +  8] == 't' &&
                                    targetstring[i +  9] == 'r' &&
                                    targetstring[i + 10] == 'e' &&
                                    targetstring[i + 11] == 'a' &&
                                    targetstring[i + 12] == 'm'
                                )
                                &&
                                (
                                    !afterDoubleSlash
                                )
                            )
                                {
                                    result += "\e[34m";
                                }
                        if  // ifstream is found
                            (
                                targetstring[  i   ] == 'i' && i + 1 < targetstring.length() &&
                                targetstring[i +  1] == 'f' &&
                                targetstring[i +  2] == 's' &&
                                targetstring[i +  3] == 't' &&
                                targetstring[i +  4] == 'r' &&
                                targetstring[i +  5] == 'e' &&
                                targetstring[i +  6] == 'a' &&
                                targetstring[i +  7] == 'm'
                            )
                                {
                                    result += "\e[34m";
                                }
                        if  // regex is found
                            (
                                targetstring[  i   ] == 'r' && i + 1 < targetstring.length() &&
                                targetstring[i +  1] == 'e' &&
                                targetstring[i +  2] == 'g' &&
                                targetstring[i +  3] == 'e' &&
                                targetstring[i +  4] == 'x'
                            )
                                {
                                    result += "\e[34m";
                                }
                        if  // char is found
                            (
                                targetstring[i] == '<' ||
                                targetstring[i] == '>' ||
                                targetstring[i] == '&' ||
                                targetstring[i] == ')' ||
                                targetstring[i] == ';'
                            )
                                {
                                    result += "\e[32m";
                                }
                        if  // endl is found
                            (
                                targetstring[  i  ] == 'e' &&
                                targetstring[i + 1] == 'n' &&
                                targetstring[i + 2] == 'd' &&
                                targetstring[i + 3] == 'l' &&
                                targetstring[i + 4] == ';'
                            )
                                {
                                    result += "\e[35m";
                                }
                        if  // whitespace is found reset color
                            (
                                targetstring[i] == ' ' ||
                                targetstring[i] == '\n'
                            )
                                {
                                    result += "\e[0m";
                                    is_std = false;
                                }
                        }
                if  /* inside quotes. */
                    ( inside_quotes )
                        {
                            if  // escape codes are found.
                                (
                                    targetstring[i] == '\\' &&
                                    (
                                        (   // escape code "n" for newline
                                            targetstring[i + 1] == 'n'
                                        )
                                        ||
                                        (   // escape code "x1b"
                                            targetstring[i + 1] == 'x' &&
                                            targetstring[i + 2] == '1' &&
                                            targetstring[i + 3] == 'B'
                                        )
                                    )
                                )
                                    {   // make blue
                                        result += "\e[94m";
                                    }
                            // reset color to red when escape code ends.
                            if ((targetstring[i - 1] == 'n' && targetstring[i - 2] == '\\') || (targetstring[i - 1] == 'B' && targetstring[i - 2] == '1' && targetstring[i - 3] == 'x' && targetstring[i - 4] == '\\')){
                                result += "\e[31m";
                            }
                        }
                if  // yellow words are found.
                (
                    (
                        (   // "if"
                            targetstring[i - 1] != '#' &&
                            targetstring[  i  ] == 'i' && i + 1 < targetstring.length() &&
                            targetstring[i + 1] == 'f' &&
                            targetstring[i + 2] == ' '
                        )
                        ||
                        (   // "break"
                            targetstring[  i  ] == 'b' && i + 1 < targetstring.length() &&
                            targetstring[i + 1] == 'r' &&
                            targetstring[i + 2] == 'e' &&
                            targetstring[i + 3] == 'a' &&
                            targetstring[i + 4] == 'k'
                        )
                        ||
                        (   // "while"
                            targetstring[  i  ] == 'w' && i + 1 < targetstring.length() &&
                            targetstring[i + 1] == 'h' &&
                            targetstring[i + 2] == 'i' &&
                            targetstring[i + 3] == 'l' &&
                            targetstring[i + 4] == 'e'
                        )
                        ||
                        (   // "else"
                            targetstring[  i  ] == 'e' && i + 1 < targetstring.length() &&
                            targetstring[i + 1] == 'l' &&
                            targetstring[i + 2] == 's' &&
                            targetstring[i + 3] == 'e'
                        )
                        ||
                        (   // "for"
                            targetstring[  i  ] == 'f' && i + 1 < targetstring.length() &&
                            targetstring[i + 1] == 'o' &&
                            targetstring[i + 2] == 'r' &&
                            targetstring[i + 3] != 'k'
                        )
                        ||
                        (   // "try"
                            targetstring[  i  ] == 't' && i + 1 < targetstring.length() &&
                            targetstring[i + 1] == 'r' &&
                            targetstring[i + 2] == 'y'
                        )
                        ||
                        (   // "catch"
                            targetstring[  i  ] == 'c' && i + 1 < targetstring.length() &&
                            targetstring[i + 1] == 'a' &&
                            targetstring[i + 2] == 't' &&
                            targetstring[i + 3] == 'c' &&
                            targetstring[i + 4] == 'h' &&
                            targetstring[i + 5] == ' '
                        )
                        ||
                        (   // "return"
                            targetstring[  i  ] == 'r' && i + 1 < targetstring.length() &&
                            targetstring[i + 1] == 'e' &&
                            targetstring[i + 2] == 't' &&
                            targetstring[i + 3] == 'u' &&
                            targetstring[i + 4] == 'r' &&
                            targetstring[i + 5] == 'n' &&
                            targetstring[i + 6] != '_'
                        )
                        ||
                        (   // "continue"
                            targetstring[  i  ] == 'c' && i + 1 < targetstring.length() &&
                            targetstring[i + 1] == 'o' &&
                            targetstring[i + 2] == 'n' &&
                            targetstring[i + 3] == 't' &&
                            targetstring[i + 4] == 'i' &&
                            targetstring[i + 5] == 'n' &&
                            targetstring[i + 6] == 'u' &&
                            targetstring[i + 7] == 'e' &&
                            targetstring[i + 8] == ';'
                        )
                    )
                    &&
                    (
                        !inside_quotes &&
                        !inside_single_quotes &&
                        !afterDoubleSlash &&
                        !include_found &&
                        !special_comment
                    )

                )
                    {
                        result += "\e[93m\e[1m";
                        yellow_found = true;
                    }
                if  // yellow_found is true
                    (yellow_found)
                        {
                            if  // whitespace is found after yellow word
                                (
                                    targetstring[i] == ' ' ||
                                    targetstring[i] == '\n'
                                )
                                    {
                                        result += "\e[0m";
                                        yellow_found = false;
                                    }
                            if
                                ( targetstring[i] == ';' )
                                    {
                                        result += "\e[32m";
                                        yellow_found = false;
                                    }
                        }
                if  // return_value is found
                    (
                        (
                            (
                                targetstring[  i  ] == 'i' &&
                                targetstring[i + 1] == 'n' &&
                                targetstring[i + 2] == 't' &&
                                targetstring[i + 3] == ' '
                            )
                            ||
                            (
                                targetstring[  i  ] == 'v' &&
                                targetstring[i + 1] == 'o' &&
                                targetstring[i + 2] == 'i' &&
                                targetstring[i + 3] == 'd' &&
                                targetstring[i + 4] == ' '
                            )
                            ||
                            (
                                targetstring[  i  ] == 'd' &&
                                targetstring[i + 1] == 'o' &&
                                targetstring[i + 2] == 'u' &&
                                targetstring[i + 3] == 'b' &&
                                targetstring[i + 4] == 'l' &&
                                targetstring[i + 5] == 'e' &&
                                targetstring[i + 6] == ' '
                            )
                            ||
                            (
                                (
                                    targetstring[  i  ] == 'c' &&
                                    targetstring[i + 1] == 'h' &&
                                    targetstring[i + 2] == 'a' &&
                                    targetstring[i + 3] == 'r'
                                )
                                &&
                                (
                                    targetstring[i + 4] == ' ' ||
                                    targetstring[i + 4] == '*'
                                )
                            )
                            ||
                            (
                                (
                                    targetstring[  i  ] == 'c' &&
                                    targetstring[i + 1] == 'o' &&
                                    targetstring[i + 2] == 'n' &&
                                    targetstring[i + 3] == 's' &&
                                    targetstring[i + 4] == 't'
                                )
                                &&
                                (
                                    targetstring[i + 5] == ' ' ||
                                    targetstring[i + 5] == '*'
                                )
                            )
                        )
                        &&
                        (
                            !inside_quotes &&
                            !inside_single_quotes &&
                            !afterDoubleSlash
                        )

                    )
                        {
                            result += "\e[34m";
                            return_value_found = true;
                        }
                if  // int_found is true
                    (return_value_found)
                        {
                            if (targetstring[i] == ' ') {
                                result += "\e[0m";
                                return_value_found = false;
                            }
                        }
                if  // blue words are found
                    (
                        (
                            (   // "auto"
                                targetstring[  i  ] == 'a' && i + 1 < targetstring.length() &&
                                targetstring[i + 1] == 'u' &&
                                targetstring[i + 2] == 't' &&
                                targetstring[i + 3] == 'o' &&
                                (
                                targetstring[i + 4] == ' ' ||
                                targetstring[i + 4] == '&'
                                )
                            )
                            ||
                            (   // "size_t"
                                targetstring[  i  ] == 's' && i + 1 < targetstring.length() &&
                                targetstring[i + 1] == 'i' &&
                                targetstring[i + 2] == 'z' &&
                                targetstring[i + 3] == 'e' &&
                                targetstring[i + 4] == '_' &&
                                targetstring[i + 5] == 't' &&
                                targetstring[i + 6] == ' '
                            )
                        )
                        &&
                        (
                            !inside_quotes &&
                            !inside_single_quotes &&
                            !afterDoubleSlash
                        )
                    )
                        {
                            result += "\e[34m";
                        }

                if  // end of blue word is found
                    (
                        (
                            (   // "auto"
                                (
                                targetstring[  i  ] == ' ' ||
                                targetstring[  i  ] == '&'
                                )
                                &&
                                (
                                targetstring[i - 1] == 'o' &&
                                targetstring[i - 2] == 't' &&
                                targetstring[i - 3] == 'u' &&
                                targetstring[i - 4] == 'a'
                                )
                            )
                            ||
                            (   // "size_t"
                                targetstring[  i  ] == ' ' &&
                                targetstring[i - 1] == 't' &&
                                targetstring[i - 2] == '_' &&
                                targetstring[i - 3] == 'e' &&
                                targetstring[i - 4] == 'z' &&
                                targetstring[i - 5] == 'i' &&
                                targetstring[i - 6] == 's'
                            )
                        )
                        &&
                        (
                            !inside_quotes &&
                            !inside_single_quotes &&
                            !afterDoubleSlash
                        )
                    )
                    {
                    result += "\e[0m";
                }
                result += targetstring[i];  // Add letter to result.
            }
            targetstring = result;  // Update the targetstring with the modified result
        }
        void nums_outside_quotes(std::string& targetstring) {
            bool inside_quotes = false;
            bool afterDoubleSlash = false;
            bool is_num = false;
            bool special_comment = false;
            std::string result; // Initialize an empty result string

            for (size_t i = 0; i < targetstring.length(); ++i) {
                if (targetstring[i] == '"') {
                    if (!inside_quotes) {
                        // Entering a quoted section
                        inside_quotes = true;
                    } else {
                        // Exiting a quoted section
                        inside_quotes = false;
                    }
                }
                if (targetstring[i] == '/' && i + 1 < targetstring.length() && targetstring[i + 1] == '/') {
                    afterDoubleSlash = true;
                }
                if ((targetstring[i] >= '0' && targetstring[i] <= '9') && !inside_quotes && !afterDoubleSlash && !special_comment) {
                    if (!is_num){
                        is_num = true;
                        result += "\033[33m";
                    }
                } else if (is_num) {
                    result += "\033[0m";
                    is_num = false;
                }
                if (afterDoubleSlash) {
                    if (targetstring[i] == '\n') {
                        afterDoubleSlash = false;
                    }
                }
                if  // start of special comment found
                    (
                        targetstring[  i  ] == '/' &&
                        targetstring[i + 1] == '*'
                    )
                        {
                            result += "\e[90m";
                            special_comment = true;
                        }
                if  // end of special comment found
                    (
                        targetstring[i - 1] == '/' &&
                        targetstring[i - 2] == '*'
                    )
                        {
                            result += "\e[0m";
                            special_comment = false;
                        }
                result += targetstring[i];
            }

            // Update the targetstring with the modified result
            targetstring = result;
        }
        void replaceSpacesAfterSlash(std::string& targetstring) {
            bool afterDoubleSlash = false;

            for (size_t i = 0; i < targetstring.length(); ++i) {
                if (targetstring[i] == '/' && i + 1 < targetstring.length() && targetstring[i + 1] == '/') {
                    afterDoubleSlash = true;
                }
                if (afterDoubleSlash) {
                    if (targetstring[i] == ' ') {
                        targetstring[i] = '_';  // Replace space with underscore
                    }
                    if (targetstring[i] == '\n') {
                        afterDoubleSlash = false;  // Stop replacing after newline
                    }
                }
            }
        }
        void reverse_replaceSpacesAfterSlash(std::string& targetstring) {
            bool afterDoubleSlash = false;

            for (size_t i = 0; i < targetstring.length(); ++i) {
                if (targetstring[i] == '/' && i + 1 < targetstring.length() && targetstring[i + 1] == '/') {
                    afterDoubleSlash = true;
                }
                if (afterDoubleSlash) {
                    if (targetstring[i] == '_') {
                        targetstring[i] = ' ';              // Replace space with underscore
                    }
                    if (targetstring[i] == '\n') {
                        afterDoubleSlash = false;  // Stop replacing after newline
                    }
                }
            }
        }
        void insertStringAtPosition(std::string& original, const std::string& toInsert, int targetLine, int targetPos) {
            std::string result;
            int lineCount = 1;
            int currentPos = 0;

            for (char character : original) {
                if (lineCount == targetLine && currentPos == targetPos) {
                    result += toInsert;  // Insert the string at the specified position
                }

                if (character == '\n') {
                    lineCount++;
                    currentPos = 0;
                } else {
                    currentPos++;
                }

                result += character;
            }

            original = result;
        }
        void quotes_red(std::string& targetstring){
            std::regex string("\\bstring\\b");
            std::regex quote("\"");
            std::regex quotePattern("\".*?\"");
            // bool inside_quotes = false;
            std::string word;
            std::istringstream iss(targetstring);
            std::string line;
            int lineNum = 1;
            int quote_num = 0;
            int length = 0;
            int last_line_num = 0;

            while (std::getline(iss, line)) {               // Loop through lines of the input string
                std::smatch match;
                std::string::const_iterator searchStart(line.cbegin());
                while (std::regex_search(searchStart, line.cend(), match, quotePattern)) {// Search for the whole word in the line
                    if (last_line_num != lineNum){
                        length = 0;
                    } else {
                        length = length + 9;
                    }
                    length = length + match.position() + 1;
                    quote_num++;
                    std::cout << "start :quote :" << quote_num << " line :" << lineNum << ":" << length << ":" << length + match.length() << "\n";
                    insertStringAtPosition(targetstring, "\e[31m", lineNum, length - 1);
                    insertStringAtPosition(targetstring, "\e[0m", lineNum, length + match.length() + 4);
                    length = length + 1;
                    searchStart = match.suffix().first;
                    last_line_num = lineNum;
                    length = length + match.length() - 2;
                }
                lineNum++;
            }
            // while (iss >> word){
            //     if (word.find('"') != std::string::npos && !inside_quotes){
            //         inside_quotes = true;
            //     } else if (word.find('"') != std::string::npos && !inside_quotes){
            //         inside_quotes = false;
            //     } else if (!inside_quotes && word == "string"){
            //         targetstring = std::regex_replace(targetstring, string, "\e[94m$&\e[0m");
            //     }
            // }
        }
        void functions(std::string& targetstring) {
            bool inside_quotes = false;
            bool afterDoubleSlash = false;
            std::string result; // Initialize an empty result string
            int length;

            for (size_t i = 0; i < targetstring.length(); ++i) {
                if (targetstring[i] == '"') {
                    if (!inside_quotes) {
                        // Entering a quoted section
                        inside_quotes = true;
                    } else {
                        // Exiting a quoted section
                        inside_quotes = false;
                    }
                }
                if (targetstring[i] == '/' && i + 1 < targetstring.length() && targetstring[i + 1] == '/') {
                    afterDoubleSlash = true;
                }
                if (targetstring[i] == ' ') {
                    length = -1;
                } else if (targetstring[i] == '.'){
                    length = -1;
                } else if (targetstring[i] == ':'){
                    length = -1;
                } else {
                    length = length + 1;
                }
                if (targetstring[i] == '(' && length >= 2 && !inside_quotes && !afterDoubleSlash) {
                    result.insert(result.length() - length, "\033[35m");
                    result += "\033[0m";
                }
                if (afterDoubleSlash) {
                    if (targetstring[i] == '\n') {
                        afterDoubleSlash = false;
                    }
                }
                result += targetstring[i];
            }

            // Update the targetstring with the modified result
            targetstring = result;
        }
    }
}
