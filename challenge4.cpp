#include "Utils.h"

#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "You must specify an input file to search encoded strings from!" << std::endl;
        return 1;
    }

    std::ifstream ifile(argv[1], std::ios_base::in);
    if (!ifile.is_open()) {
        std::cout << "Unable to open file for read: " << argv[1] << std::endl;
        return 2;
    }

    std::cout << "Looking for encoded string in: " << argv[1] << std::endl;

    float best = 0.0f;
    unsigned char encode_byte = 0;
    std::string result;
    std::string original;

    for(std::string line; std::getline(ifile, line); ) {
        std::string bytes = hex::decode(line);
        for(unsigned char i = 0; i < 255; ++i) {
            std::string decoded = cipher::byte_xor(bytes, i);
            float score = ascii::frequency_score(decoded);
            if (score > best) {
                best = score;
                encode_byte = i;
                result = decoded;
                original = line;
            }
        }
    }

    std::cout << std::endl;
    std::cout << "Encoded: " << original << std::endl;
    std::cout << "Decoded: " << result << std::endl;
    std::cout << "Encode Byte: 0x" << hex::encode(encode_byte) << std::endl;
    std::cout << "Score: " << best << std::endl;


    ifile.close();

    return 0;
}
