#include "file_utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>

namespace hazmat::io {

    std::string FetchFile(const std::string& filepath)
    {
        std::ifstream fileStream{ filepath };
        if (!fileStream.is_open())
        {
            // HZ_WARN("Could not open file by the name of '{}'", filepath.c_str());
            std::cout << "Could not open file by the name of '" << filepath << "'\n";
            return "";
        }

        return std::string((std::istreambuf_iterator<char>(fileStream)),
            (std::istreambuf_iterator<char>()));
    }

}
