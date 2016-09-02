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
    // Basically, treat the array of bytes as a contiguous sequence of bits
    // and encode every 6 bits starting at the begining of the string into
    // a unit (byte sized value).  The unit  maps to a particular ASCII character 
    // for encoding from the _base64Table.

    const char* cur = input.c_str();
    const char* end = cur + input.size();

    const int UNIT_MASK = 0x3F;
    const int BYTES_PER_WORD = 3;

    std::string output;
    
    // Iterate in 3 byte chunks, shift them into a 4 byte integer for
    // a total of 18bits of data.
    //
    // Then split the 18 bits into 4 6bit unit.  Map each 6bit unit to
    // a unique ASCII character from the mapping.  Left over bytes 
    // will be handled after the loop.
    char bytes[BYTES_PER_WORD] = { 0 };
    while ((cur + BYTES_PER_WORD) <= end) {
        bytes[0] = *cur;
        bytes[1] = *(cur + 1);
        bytes[2] = *(cur + 2);

        unsigned int word = (bytes[0] << 16) | (bytes[1] << 8) | (bytes[2]);

        unsigned char u0 = (word >> 18) & UNIT_MASK;
        unsigned char u1 = (word >> 12) & UNIT_MASK;
        unsigned char u2 = (word >> 6) & UNIT_MASK;
        unsigned char u3 = word & UNIT_MASK;

        output += _base64Table[u0];
        output += _base64Table[u1];
        output += _base64Table[u2];
        output += _base64Table[u3];

        cur += BYTES_PER_WORD;
    }

    size_t bytes_left = end - cur;

    if (bytes_left == 1) {
        // If we have a remainder of 1 byte, extract 2 6bit units from
        // the last byte.  The high order 6 are used as the first number
        // and the remaining 2 low order bits are used as the 2 high order
        // bits for the remaining 6bit unit
        char byte = *cur;
        unsigned char u0 = (byte & 0xFC) >> 2;
        unsigned char u1 = (byte & 0x03) << 4;
        output += _base64Table[u0];
        output += _base64Table[u1];
        output += "==";
    }
    else if (bytes_left == 2) {
        // Do the same as above, but we have an extra 8bits to work with.
        // Shit the high order 4 bits of the last byte into the low order 4 bits of
        // our second 6bit unit.  Then the last 6bit unit consists of the remaining 4
        // bits of the last byte.
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

std::string decode(const std::string& input) {
    // Create a reverse lookup table from base64 ASCII values to
    // the correct integer values
    char lookup[256] = { 0 };
    for (size_t i = 0; i < sizeof(_base64Table); ++i) {
        lookup[ static_cast<size_t>(_base64Table[i]) ] = i;
    }

    const char* cur = input.c_str();
    const char* end = cur + input.size();

    const int BYTES_PER_PASS = 4;

    std::string output;

    // Iterate in 4 byte chunks, each byte maps to a value that will not
    // exceede 6bits. Combine the bits into a 32bit integer and pull out 
    // the first three bytes worth of data starting from the high order bits.
    //
    // Pad values map to 0 which will automatically be ignored
    while((cur + BYTES_PER_PASS) <= end) {
        unsigned char u0 = lookup[ static_cast<size_t>(*cur) ];
        unsigned char u1 = lookup[ static_cast<size_t>(*(cur + 1)) ];
        unsigned char u2 = lookup[ static_cast<size_t>(*(cur + 2)) ];
        unsigned char u3 = lookup[ static_cast<size_t>(*(cur + 3)) ];

        // All 6 bits of u0 or'd with top 2 bits of u1
        char byte0 = (u0 << 2) | (u1 >> 4);

        // Bottom 4 bits of u1 or'd with top 4 bits of u2
        char byte1 = ((u1 & 0xF) << 4) | (u2 >> 2);

        // Bottom 2 bits of u2 or'd with all 6 bits of u3
        char byte2 = ((u2 & 0x3) << 6) | u3;

        output += byte0;
        output += byte1;
        output += byte2;

        cur += BYTES_PER_PASS;
    }

    return output;
}
} // namespace base64

namespace hex {
std::string encode(const std::string& input) {
    const char* lookup = "0123456789abcdef";

    std::string result;
    for(size_t i = 0; i < input.size(); ++i) {
        char byte = input[i]; 
        result += lookup[ static_cast<size_t>(((byte & 0xF0) >> 4)) ];
        result += lookup[ static_cast<size_t>(byte & 0xF) ];
    }

    return result;
}

std::string decode(const std::string& input) {
    const char* lookup = "0123456789abcdef";
    char hex2dec[256] = {0};
    for(size_t i = 0; i < 16; ++i) {
        hex2dec[static_cast<size_t>(lookup[i])] = i;
    }

    std::string result;
    char byte = 0;
    for(size_t i = 0; (i + 2) <= input.size(); i+=2) {
        byte = hex2dec[ static_cast<size_t>(input[i]) ] << 4;
        byte = byte | hex2dec[ static_cast<size_t>(input[i+1]) ];
        result += byte;
    }

    return result;
}
} // namespace hex

namespace cipher {
    std::string fixed_xor(const std::string& buf1, const std::string& buf2) {
        std::string result;
        if (buf1.size() == buf2.size()) {
            result.reserve(buf1.size());
            for(size_t i = 0; i < buf1.size(); ++i) {
                result += buf1[i] ^ buf2[i];
            }
        }
        return result;
    }
} // namespace cipher

