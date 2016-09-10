#include "Utils.h"

#include <iostream>
#include <fstream>

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Please specify the challenege6 file!" << std::endl;
        return 1;
    }

    std::ifstream ifile(argv[1], std::ios_base::in);
    if (!ifile.is_open()) {
        std::cout << "Unable to open file: " << argv[1] << std::endl;
        return 2;
    }

    // Load each line, base64 decode it and append it to the list of
    // bytes to operate on
    std::string bytes;
    for(std::string line; std::getline(ifile, line); ) {
        bytes += base64::decode(line);
    }

    ifile.close();


    openssl::init();

    std::cout << openssl::decrypt(bytes, "YELLOW SUBMARINE", EVP_aes_128_ecb()) << std::endl;

    openssl::cleanup();

    return 0;
}


