#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sys/stat.h>
#include <ctime>
#include <unistd.h>

class MailChecker {
private:
    struct FileInfo {
        std::string name;
        std::string msg;
        time_t access_time;
        time_t mod_time;
        off_t file_size;
        int flags;
    };

    std::vector<FileInfo> mailfiles;
    time_t last_time_mail_checked;
    const int MBOX_INITIALIZED = 0x01;

public:
    MailChecker() : last_time_mail_checked(0) {}

    bool time_to_check_mail() {
        const char* temp = std::getenv("MAILCHECK");
        if (temp == nullptr) return false;

        intmax_t seconds = std::stoi(temp);
        if (seconds < 0) return false;

        time_t now = std::time(nullptr);
        return seconds == 0 || ((now - last_time_mail_checked) >= seconds);
    }

    void reset_mail_timer() {
        last_time_mail_checked = std::time(nullptr);
    }

    void add_mail_file(const std::string& filename, const std::string& msg) {
        struct stat finfo;
        if (stat(filename.c_str(), &finfo) == 0) {
            FileInfo file_info{filename, msg, finfo.st_atime, finfo.st_mtime, finfo.st_size, MBOX_INITIALIZED};
            mailfiles.push_back(file_info);
        }
    }

    void check_mail() {
        for (auto& file_info : mailfiles) {
            struct stat finfo;
            if (stat(file_info.name.c_str(), &finfo) == 0) {
                if (finfo.st_mtime > file_info.mod_time) {
                    std::cout << "You have new mail in " << file_info.name << std::endl;
                    file_info.access_time = finfo.st_atime;
                    file_info.mod_time = finfo.st_mtime;
                    file_info.file_size = finfo.st_size;
                }
            }
        }
    }
};

int main() {
    MailChecker mail_checker;
    // Initialize the mail files here.
    mail_checker.add_mail_file("/path/to/mail/file1", "You have new mail!");

    while (true) {
        if (mail_checker.time_to_check_mail()) {
            mail_checker.check_mail();
            mail_checker.reset_mail_timer();
        }
        sleep(5); // Check every 5 seconds for demonstration purposes
    }

    return 0;
}
