#include "Utils.h"

#include <iostream>

int main(int argc, char** argv) {
    if (argc > 1) {
        std::string bytes = hex::decode(argv[1]);
        std::cout << "Output: " << base64::encode(bytes) << std::endl;
    }

    return 0;
}
