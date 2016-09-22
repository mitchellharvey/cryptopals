#include "Utils.h"

#include <iostream>
#include <fstream>

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Please specify the challenege10 file!" << std::endl;
        return 1;
    }

    std::ifstream ifile(argv[1], std::ios_base::in);
    if (!ifile.is_open()) {
        std::cout << "Unable to open file: " << argv[1] << std::endl;
        return 2;
    }

    std::string bytes;
    for(std::string line; std::getline(ifile, line); ) {
        bytes += (base64::decode(line));
    }

    ifile.close();

    std::string key = "YELLOW SUBMARINE";
    std::string iv(16, '\0');

    openssl::init();

    std::cout << openssl::cbc_decrypt(bytes, key, iv) << std::endl;

    openssl::cleanup();

    return 0;
}

