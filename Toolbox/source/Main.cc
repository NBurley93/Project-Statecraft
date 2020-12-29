#include <iostream>
#include <string>
#include <filesystem>

const std::filesystem::path CONTENT_PATH = std::filesystem::current_path().append("game");

void printUsage() {
    std::cout << "usage: toolbox verb <options..>" << std::endl;
    std::cout << "  bitmap <source-file>" << std::endl;
}

int main(int argc, char* argv[]) {
    printUsage();
    return 0;
}