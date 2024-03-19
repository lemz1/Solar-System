#include "AssetManager.h"

#include <iostream>
#include <fstream>
#include <cassert>

std::string AssetManager::GetFileContent(const char* filePath)
{
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Couldn't find file: " << filePath << std::endl;
        #ifdef DEBUG
        assert(false);
        #endif
        return "";
    }

    std::string content;
    file.seekg(0, std::ios::end);
    content.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&content[0], content.size());
    file.close();

    return content;
}
