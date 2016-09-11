#include "Utils.h"

#include <iostream>

int main(int argc, char** argv) {

    if (argc < 3) {
        std::cout << "You must specify a string to pad and the block size in bytes." << std::endl;
        std::cout << "\tE.g: " << argv[0] << " \"Some cool string\" 16" << std::endl;
        return 1;
    }

    std::string input(argv[1]);
    unsigned char pad_size = static_cast<unsigned char>(std::atoi(argv[2]));
    std::cout << hex::encode(cipher::pad_pkcs7(input, pad_size)) << std::endl;
    std::cout << input << std::endl;

    return 0;
}

