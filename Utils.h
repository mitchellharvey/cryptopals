#pragma once
#include <string>

namespace base64 {
    std::string encode(const std::string& input);
    std::string decode(const std::string& input);
}

namespace hex {
    std::string encode(const std::string& input);
    std::string decode(const std::string& input);
}

namespace cipher {
    std::string fixed_xor(const std::string& buf1, const std::string& buf2);
}

