#include <fstream>
#include <iostream>
#include <sstream>

#include "json-parser/JSONParser.h"

int main() {
    // Open the file
    std::ifstream file("input.json");

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Unable to open the file." << std::endl;
        return 1;
    }

    // Read the file into a stringstream
    std::stringstream buffer;
    buffer << file.rdbuf();

    // Close the file
    file.close();

    // Get the content of the stringstream as a string
    std::string input = buffer.str();

    // Output the content
    // std::cout << input << std::endl;

    JSONParser parser(input);
    auto value = parser.parse();

    if (!value.isError()) {
        std::cout << "Done Lexing." << std::endl;
        // value.value()->print(0);
    } else {
        std::cout << value.errorMessage() << std::endl;
    }
    return 0;
}