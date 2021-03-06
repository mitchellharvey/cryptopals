#pragma once
#include <openssl/evp.h>
#include <string>

namespace base64 {
    std::string encode(const std::string& bytes);
    std::string encode(unsigned char byte);

    std::string decode(const std::string& bytes);
    std::string decode(unsigned char byte);
}

namespace hex {
    std::string encode(const std::string& bytes);
    std::string encode(unsigned char byte);

    std::string decode(const std::string& bytes);
    std::string decode(unsigned char byte);
}

namespace cipher {
    std::string fixed_xor(const std::string& bytes1, const std::string& bytes2);
    std::string byte_xor(const std::string& bytes, unsigned char byte);
    std::string repeating_xor(const std::string& bytes, const std::string& repeat);
    double hamming_distance(const std::string& bytes1, const std::string& bytes2);
    unsigned char guess_xor_byte(const std::string& bytes, float* out_score = nullptr);
    std::string pad_pkcs7(const std::string& bytes, unsigned char block_size);
    std::string unpad_pkcs7(const std::string& bytes, unsigned char block_size);
}

namespace openssl {
    void init();
    void cleanup();
    std::string decrypt(const std::string& bytes, const std::string& key, const EVP_CIPHER* mode, int padding = -1);
    std::string encrypt(const std::string& bytes, const std::string& key, const EVP_CIPHER* mode, int padding = -1);
    std::string cbc_encrypt(const std::string& bytes, const std::string& key, const std::string& iv);
    std::string cbc_decrypt(const std::string& bytes, const std::string& key, const std::string& iv);
    std::string random(size_t byte_count);
    unsigned long random_number(int min, int max);
}

namespace ascii {
    bool alpha_numeric(const std::string& bytes);
    bool alpha_numeric(unsigned char byte);

    bool punctuation(unsigned char byte);
    bool punctuation(const std::string& bytes);

    bool printable(unsigned char byte);
    bool printable(const std::string& bytes);

    float frequency_score(const std::string& bytes);
}

