#include "Utils.h"

#include <iostream>
#include <unordered_map>
#include <fstream>

std::string oracle(const std::string& input, const std::string& key) {
    return openssl::encrypt(input, key, EVP_aes_128_ecb());
}

size_t get_block_size(const std::string& msg, const std::string& key) {
    
    std::string crypted = oracle(msg, key);

    size_t original_size = crypted.size();

    const size_t MAX_BLOCK_SIZE = 255;
    for(size_t i = 0; i < MAX_BLOCK_SIZE; ++i) {
        std::string test = oracle(std::string(i, 'A') + msg, key);
        if (test.size() > original_size) {
            return test.size() - original_size;
        }
    }

    return 0;
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

    std::string key = openssl::random(16);
    std::string message =   "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkg"
                            "aGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBq"
                            "dXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUg"
                            "YnkK";

    openssl::init();

    message = base64::decode(message);

    size_t block_size = get_block_size(message, key);
    bool ecb = is_ecb(oracle(std::string(block_size * 2, 'A') + message, key), block_size);
    std::cout << "Block Size: " << block_size << std::endl;
    std::cout << "Is Ecb: " << ecb << std::endl;

    // Build dictionary of every encrypted byte value to the 
    // unencrypted byte value
    std::unordered_map<std::string, char> dic;
    for(size_t i = 0; i < 256; ++i) {
        std::string app(block_size - 1, 'A');
        app += static_cast<char>(i) + message;
        std::string enc = oracle(app, key);
        dic[enc.substr(0, block_size)] = static_cast<char>(i);
    }

    // Decrypt the original encrypted message one byte at a time
    std::string encrypted = oracle(message, key);
    std::string result;
    for(size_t i = 0; i < encrypted.size(); ++i) {
        std::string temp = oracle(std::string(block_size - 1, 'A') + (message.c_str() + i), key);
        result += dic[temp.substr(0, block_size)];
    }

    std::cout << "Result:" << std::endl << result << std::endl;

    openssl::cleanup();

    return 0;
}

