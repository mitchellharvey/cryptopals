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
std::string encode(unsigned char byte) {
    return encode(std::string(reinterpret_cast<char*>(&byte), 1));
}

std::string encode(const std::string& bytes) {
    // Basically, treat the array of bytes as a contiguous sequence of bits
    // and encode every 6 bits starting at the begining of the string into
    // a unit (byte sized value).  The unit  maps to a particular ASCII character 
    // for encoding from the _base64Table.

    const char* cur = bytes.c_str();
    const char* end = cur + bytes.size();

    const int UNIT_MASK = 0x3F;
    const int BYTES_PER_WORD = 3;

    std::string output;
    
    // Iterate in 3 byte chunks, shift them into a 4 byte integer for
    // a total of 18bits of data.
    //
    // Then split the 18 bits into 4 6bit unit.  Map each 6bit unit to
    // a unique ASCII character from the mapping.  Left over bytes 
    // will be handled after the loop.
    char b[BYTES_PER_WORD] = { 0 };
    while ((cur + BYTES_PER_WORD) <= end) {
        b[0] = *cur;
        b[1] = *(cur + 1);
        b[2] = *(cur + 2);

        unsigned int word = (b[0] << 16) | (b[1] << 8) | (b[2]);

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

std::string decode(unsigned char byte) {
    return decode(std::string(reinterpret_cast<char*>(&byte), 1));
}

std::string decode(const std::string& bytes) {
    // Create a reverse lookup table from base64 ASCII values to
    // the correct integer values
    char lookup[255] = { 0 };
    for (size_t i = 0; i < sizeof(_base64Table); ++i) {
        lookup[ static_cast<size_t>(_base64Table[i]) ] = i;
    }

    const char* cur = bytes.c_str();
    const char* end = cur + bytes.size();

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
std::string encode(unsigned char byte) {
    return encode(std::string(reinterpret_cast<char*>(&byte), 1));
}

std::string encode(const std::string& bytes) {
    const char* lookup = "0123456789abcdef";

    std::string result;
    for(size_t i = 0; i < bytes.size(); ++i) {
        char byte = bytes[i]; 
        result += lookup[ static_cast<size_t>(((byte & 0xF0) >> 4)) ];
        result += lookup[ static_cast<size_t>(byte & 0xF) ];
    }

    return result;
}

std::string decode(unsigned char byte) {
    return decode(std::string(reinterpret_cast<char*>(&byte), 1));
}

std::string decode(const std::string& bytes) {
    const char* lookup = "0123456789abcdef";
    char hex2dec[255] = {0};
    for(size_t i = 0; i < 16; ++i) {
        hex2dec[static_cast<size_t>(lookup[i])] = i;
    }

    std::string result;
    char byte = 0;
    for(size_t i = 0; (i + 2) <= bytes.size(); i+=2) {
        byte = hex2dec[ static_cast<size_t>(bytes[i]) ] << 4;
        byte = byte | hex2dec[ static_cast<size_t>(bytes[i+1]) ];
        result += byte;
    }

    return result;
}
} // namespace hex

namespace cipher {
std::string fixed_xor(const std::string& bytes1, const std::string& bytes2) {
    std::string result;
    if (bytes1.size() == bytes2.size()) {
        result.reserve(bytes1.size());
        for(size_t i = 0; i < bytes1.size(); ++i) {
            result += bytes1[i] ^ bytes2[i];
        }
    }
    return result;
}

std::string byte_xor(const std::string& bytes, unsigned char byte) {
    std::string result(bytes.size(), 0);
    for(size_t i = 0; i < bytes.size(); ++i) {
        result[i] = bytes[i] ^ byte;
    }
    return result;
}
} // namespace cipher

namespace ascii {
bool alpha_numeric(unsigned char byte) {
    if (byte >= '0' && byte <= '9')
        return true;

    if (byte >= 'A' && byte <= 'Z')
        return true;

    if (byte >= 'a' && byte <= 'z')
        return true;

    return false;
}

bool alpha_numeric(const std::string& bytes) {
    for(char c : bytes) {
        if (alpha_numeric(c))
            continue;

        return false;
    }
    return true;
}

bool punctuation(unsigned char byte) {
    if (byte >= '\t' && byte <= '\n')
        return true;

    if (byte >= ' ' && byte <= '/')
        return true;

    if (byte >= ':' && byte <= '@')
        return true;

    if (byte >= '[' && byte <= '^')
        return true;

    if (byte >= '{' && byte <= '~')
        return true;

    return false;
}

bool punctuation(const std::string& bytes) {
    for(char c : bytes) {
        if (punctuation(c))
            continue;

        return false;
    }
    return true;
}

bool printable(unsigned char byte) {
    if (alpha_numeric(byte) || punctuation(byte))
        return true;

    return false;
}

bool printable(const std::string& bytes) {
    for(char c : bytes) {
        if (printable(c))
            continue;

        return false;
    }
    return true;
}

float frequency_score(const std::string& bytes) {
    float scores[255] = {0};
    const float UPPERCASE_SCALE = 0.1f;
    scores['a'] = 8.167f; scores['A'] = scores['a'] * UPPERCASE_SCALE;
    scores['b'] = 1.492f; scores['B'] = scores['b'] * UPPERCASE_SCALE;
    scores['c'] = 2.782f; scores['C'] = scores['c'] * UPPERCASE_SCALE;
    scores['d'] = 4.253f; scores['D'] = scores['d'] * UPPERCASE_SCALE;
    scores['e'] = 12.702f; scores['E'] = scores['e'] * UPPERCASE_SCALE;
    scores['f'] = 2.228f; scores['F'] = scores['f'] * UPPERCASE_SCALE;
    scores['g'] = 2.015f; scores['G'] = scores['g'] * UPPERCASE_SCALE;
    scores['h'] = 6.094f; scores['H'] = scores['h'] * UPPERCASE_SCALE;
    scores['i'] = 6.966f; scores['I'] = scores['i'] * UPPERCASE_SCALE;
    scores['j'] = 0.153f; scores['J'] = scores['j'] * UPPERCASE_SCALE;
    scores['k'] = 0.772f; scores['K'] = scores['k'] * UPPERCASE_SCALE;
    scores['l'] = 4.025f; scores['L'] = scores['l'] * UPPERCASE_SCALE;
    scores['m'] = 2.406f; scores['M'] = scores['m'] * UPPERCASE_SCALE;
    scores['n'] = 6.749f; scores['N'] = scores['n'] * UPPERCASE_SCALE;
    scores['o'] = 7.507f; scores['O'] = scores['o'] * UPPERCASE_SCALE;
    scores['p'] = 1.929f; scores['P'] = scores['p'] * UPPERCASE_SCALE;
    scores['q'] = 0.095f; scores['Q'] = scores['q'] * UPPERCASE_SCALE;
    scores['r'] = 5.987f; scores['R'] = scores['r'] * UPPERCASE_SCALE;
    scores['s'] = 6.327f; scores['S'] = scores['s'] * UPPERCASE_SCALE;
    scores['t'] = 9.056f; scores['T'] = scores['t'] * UPPERCASE_SCALE;
    scores['u'] = 2.758f; scores['U'] = scores['u'] * UPPERCASE_SCALE;
    scores['v'] = 0.978f; scores['V'] = scores['v'] * UPPERCASE_SCALE;
    scores['w'] = 2.361f; scores['W'] = scores['w'] * UPPERCASE_SCALE;
    scores['x'] = 0.150f; scores['X'] = scores['x'] * UPPERCASE_SCALE;
    scores['y'] = 1.974f; scores['Y'] = scores['y'] * UPPERCASE_SCALE;
    scores['z'] = 0.074f; scores['Z'] = scores['z'] * UPPERCASE_SCALE;

    float result = 0.0f;
    for(unsigned char b : bytes) {
        if (!printable(b))
            return 0;

        result += scores[b];
    }

    return result;
}
} //namespace ascii
