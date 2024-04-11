#include "c_gz.h"
#include "base_tools.h"


const size_t BLOCK_SIZE = 512;

void writeTarHeader(std::ofstream &outStream, const std::string& fileName, size_t fileSize, char entryType) {
    char header[BLOCK_SIZE] = {0};
    strncpy(header, fileName.c_str(), 99);
    snprintf(header + 100, 8, "%07o", (entryType == '5') ? 0755 : 0644);
    snprintf(header + 108, 8, "%07o", 0);
    snprintf(header + 116, 8, "%07o", 0);
    snprintf(header + 124, 12, "%011lo", static_cast<unsigned long>(fileSize));
    snprintf(header + 136, 12, "%011lo", static_cast<unsigned long>(std::time(nullptr)));
    header[156] = entryType;

    // Prepare checksum field and calculate checksum
    memset(header + 148, ' ', 8);
    unsigned int checksum = 0;
    for (size_t i = 0; i < BLOCK_SIZE; ++i) {
        checksum += static_cast<unsigned char>(header[i]);
    }
    snprintf(header + 148, 8, " %06o", checksum);

    outStream.write(header, BLOCK_SIZE);
}

void writeTarPadding(std::ofstream &outStream, size_t fileSize) {
    size_t paddingSize = (BLOCK_SIZE - (fileSize % BLOCK_SIZE)) % BLOCK_SIZE;
    std::vector<char> padding(paddingSize, '\0');
    outStream.write(padding.data(), paddingSize);
}

void writeFileToTar(std::ofstream &outFile, const std::string &filePath, const std::string &fileName) {
    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Cannot open file: " << filePath << std::endl;
        return;
    }

    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    writeTarHeader(outFile, fileName, fileSize, '0');


    std::vector<char> buffer(fileSize);
    inFile.read(buffer.data(), fileSize);
    outFile.write(buffer.data(), fileSize);

    writeTarPadding(outFile, fileSize);
}

bool compressFolder(std::ofstream &outFile, const std::string& folderPath, const std::string& rootDir = "") {
    DIR* dir = opendir(folderPath.c_str());
    if (dir == nullptr) {
        std::cerr << "Cannot open directory.\n";
        return false;
    }

    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        std::string filePath = folderPath + "/" + entry->d_name;
        std::string newRootDir = rootDir.empty() ? entry->d_name : (rootDir + "/" + entry->d_name);

        if (entry->d_type == DT_REG) {
            writeFileToTar(outFile, filePath, newRootDir);
        } else if (entry->d_type == DT_DIR) {
            writeTarHeader(outFile, newRootDir + "/", 0, '5');
            writeTarPadding(outFile, 0);
            compressFolder(outFile, filePath, newRootDir);
        }
    }

    closedir(dir);
    return true;
}

void gz(const std::vector<std::string>& args) {
    std::ofstream outFile(args::processArgs(args, "-o"), std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Cannot open output tar file.\n";
        return;
    }

    if (args::find_arg(args, "-sF")) {
        if (compressFolder(outFile, args::processArgs(args, "-sF"))) {
            std::cout << "SUCCESSFULL\n";
        } else {
            std::cout << "Failed to compress\n";
        }
    }

    outFile.close();
}
