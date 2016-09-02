#include "Utils.h"

#include <iostream>

int main(int argc, char** argv) {
    std::string input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    
    std::cout << "Hex decode then Base64 encode: " << input << std::endl;
    std::cout << "Output: " << base64::encode(hex::decode(input)) << std::endl;

    return 0;
}
