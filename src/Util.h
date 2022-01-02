#include <fstream>
#include <vector>
#include <string>
#include "constants.h"

class Util {
public:
    std::vector<DSK_BYTE> read_file(const std::string& filename);
};

