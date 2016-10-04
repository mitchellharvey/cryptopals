#include "Utils.h"

#include <iostream>
#include <fstream>

std::string oracle(const std::string& input) {
    std::string result;
    std::string key = openssl::random(16);

    // 5-10 random bytes prepended and 5-10 random bytes
    // appended to the original plain text
    std::string mangled_input = openssl::random(
                                    openssl::random_number(5, 10)) +
                                    input +
                                openssl::random(
                                    openssl::random_number(5, 10));

    // Randomly encrypt w/ ECB or CBC
    std::string encrypted;
    if (openssl::random_number(0, 1)) {
        std::cout << "Encrypt ECB" << std::endl;
        encrypted = openssl::encrypt(mangled_input, key, EVP_aes_128_ecb(), 16);
    } else {
        std::cout << "Encrypt CBC" << std::endl;
        encrypted = openssl::cbc_encrypt(mangled_input, key, openssl::random(16));
    }

    return encrypted;
}

bool is_ecb(const std::string& encrypted, size_t block_size) {
    size_t matches = 0;
    for(size_t i = 0; i < encrypted.size(); i += block_size) {
        std::string chunk(encrypted.c_str() + i, block_size);
        size_t start = i + block_size;
        while(encrypted.find(chunk, start) != std::string::npos) {
            matches++;
            start += block_size;
        }
    }

    return matches > 0;
}

int main(int argc, char** argv) {

    std::string enc = oracle(std::string('A', 100));
    std::cout << hex::encode(enc) << std::endl;

    if (is_ecb(enc, 16)) {
        std::cout << "Detected ECB" << std::endl;
    } else {
        std::cout << "Detected CBC" << std::endl;
    }

    return 0;
}

