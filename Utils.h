#pragma once
#include <string>

namespace base64 {
    std::string encode(const std::string& bytes);
    std::string decode(const std::string& bytes);
}

namespace hex {
    std::string encode(const std::string& bytes);
    std::string decode(const std::string& bytes);
}

namespace cipher {
    std::string fixed_xor(const std::string& bytes1, const std::string& bytes2);
    std::string byte_xor(const std::string& bytes, unsigned char byte);
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

