#include "Utils.h"

namespace {
    const char _base64Table[] = {
        // 0 - 25
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        // 26 - 51
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        // 52 - 61
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        // 62 - 63
        '+', '/',
    };
}

namespace base64 {
    std::string encode(const std::string& input) {
        const char* cur = input.c_str();
        const char* end = cur + input.size();

        const int BIT_MASK = 0x3F;
        const int BYTES_PER_WORD = 3;

        std::string output;
        char bytes[BYTES_PER_WORD] = { 0 };
        while ((cur + BYTES_PER_WORD) <= end) {
            bytes[0] = *cur;
            bytes[1] = *(cur + 1);
            bytes[2] = *(cur + 2);

            unsigned int word = (bytes[0] << 16) | (bytes[1] << 8) | (bytes[2]);

            unsigned char u0 = (word >> 18) & BIT_MASK;
            unsigned char u1 = (word >> 12) & BIT_MASK;
            unsigned char u2 = (word >> 6) & BIT_MASK;
            unsigned char u3 = word & BIT_MASK;

            output += _base64Table[u0];
            output += _base64Table[u1];
            output += _base64Table[u2];
            output += _base64Table[u3];

            cur += BYTES_PER_WORD;
        }

        size_t bytes_left = end - cur;

        if (bytes_left == 1) {
            char byte = *cur;
            unsigned char u0 = (byte & 0xFC) >> 2;
            unsigned char u1 = (byte & 0x03) << 4;
            output += _base64Table[u0];
            output += _base64Table[u1];
            output += "==";
        }
        else if (bytes_left == 2) {
            char byte1 = *cur;
            char byte2 = *(cur + 1);
            unsigned char u0 = (byte1 & 0xFC) >> 2;
            unsigned char u1 = ((byte1 & 0x03) << 4) | (byte2 >> 4);
            unsigned char u2 = (byte2 & 0xF) << 2;
            output += _base64Table[u0];
            output += _base64Table[u1];
            output += _base64Table[u2];
            output += "=";
        }

        return output;
    }
}

namespace hex {

    std::string encode(const std::string& input) {
        const char* lookup = "0123456789absdef";

        std::string result;
        for(size_t i = 0; i < input.size(); ++i) {
            char byte = input[i]; 
            result += lookup[((byte & 0xF0) >> 4)];
            result += lookup[byte & 0xF];
        }

        return result;
    }

    std::string decode(const std::string& input) {
        const char* lookup = "0123456789abcdef";
        char hex2dec[256] = {0};
        for(size_t i = 0; i < 16; ++i) {
            hex2dec[lookup[i]] = i;
        }

        std::string result;
        char byte = 0;
        for(size_t i = 0; (i + 2) <= input.size(); i+=2) {
            byte = hex2dec[input[i]] << 4;
            byte = byte | hex2dec[input[i+1]];
            result += byte;
        }

        return result;
    }
}

