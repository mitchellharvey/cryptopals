#include "Utils.h"

#include <iostream>

int main(int argc, char** argv) {

    std::string encoded_string = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    std::string bytes = hex::decode(encoded_string);

    std::cout << "Attempt to decode: " << encoded_string << std::endl;


    std::string result = encoded_string;
    float best = 0.0f;
    char encode_byte = '\0';
    for(unsigned char i = 0; i < 255; ++i) {
        std::string decoded = cipher::byte_xor(bytes, i);
        float score = ascii::frequency_score(decoded);

        if (score > best) {
            best = score;
            encode_byte = i;
            result = decoded;
        }
    }

    std::cout << std::endl;
    std::cout << "Score: " << best << std::endl;
    std::cout << "Byte: 0x" << hex::encode(encode_byte) << std::endl;
    std::cout << "Result: " << result << std::endl;

    return 0;
}

