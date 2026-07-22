#include "lexer.hpp"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <print>
#include <sstream>
#include <string>
#include <utility>

std::string read_file_contents(const std::string& filename);

int main(int argc, char* argv[]) {
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::setvbuf(stderr, nullptr, _IONBF, 0);

    if (argc < 3) {
        std::println(stderr, "Usage: ./your_program tokenize <filename>");
        return 1;
    }

    const std::string command = argv[1];

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);

        Lexer lexer(std::move(file_contents));
        auto tokens = lexer.tokenize();
    } else {
        std::println(stderr, "Unknown command: {}", command);
        return 1;
    }

    return 0;
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::println(stderr, "Error reading file: {}", filename);
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
