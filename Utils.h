#pragma once
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
    std::string repeating_xor(const std::string& bytes, const std::string& key);
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

