#include "Util.h"

std::vector<DSK_BYTE> Util::read_file(const std::string& filename) {
    std::streampos fileSize;
    std::ifstream file(filename, std::ios::binary);
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<DSK_BYTE> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    return fileData;
}