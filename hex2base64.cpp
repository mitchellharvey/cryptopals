#include "Utils.h"

#include <iostream>

int main(int argc, char** argv) {
    if (argc > 1) {
        std::cout << base64::encode(argv[1]) << std::endl;
        std::string hexEncoded = hex::encode(argv[1]);
        std::cout << hexEncoded << std::endl;
        std::cout << hex::decode(hexEncoded) << std::endl;
       
    }

    return 0;
}
