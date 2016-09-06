#include "Utils.h"

#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char** argv) {

    std::string input = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    std::string key = "ICE";

    std::cout << "Repeating XOR encrypt input: " << std::endl;
    std::cout << input << std::endl << std::endl;

    std::cout << "Key: " << key << std::endl << std::endl;

    std::cout << "Result: " << std::endl;
    std::cout <<  hex::encode(cipher::repeating_xor(input, key)) << std::endl;

    return 0;
}

