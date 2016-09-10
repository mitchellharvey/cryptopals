#include "Utils.h"

#include <openssl/err.h>
#include <openssl/conf.h>
#include <bitset>

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

    inline size_t IDX(char b) {
        return static_cast<size_t>(b);
    }
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

std::string repeating_xor(const std::string& bytes, const std::string& key) {
    std::string result(bytes.size(), 0);
    for(size_t i = 0; i < bytes.size(); ++i) {
        result[i] = bytes[i] ^ key[i % key.size()];
    }

    return result;
}

size_t hamming_distance(const std::string& bytes1, const std::string& bytes2) {
    size_t result = 0;
    size_t i = 0;
    for(; i < bytes1.size() && i < bytes2.size(); i++) {
        std::bitset<8> bs(bytes1[i] ^ bytes2[i]);
        result += bs.count();
    }

    // Count the remaining set bits in the bytes of the larger string
    const std::string* bigger = &bytes1;
    if (bytes2.size() > bytes1.size())
        bigger = &bytes2;

    for( ; i < bigger->size(); ++i) {
        result += std::bitset<8>((*bigger)[i]).count();
    }

    return result;
}

unsigned char guess_xor_byte(const std::string& bytes, float* out_score) {
    unsigned char encode_byte = 0;
    float best = 0;
    for(unsigned char i = 0; i < 255; ++i) {
        std::string decoded = cipher::byte_xor(bytes, i);
        float score = ascii::frequency_score(decoded);
        if (score > best) {
            best = score;
            encode_byte = i;
        }
    }

    if (out_score)
        *out_score = best;

    return encode_byte;
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
    const float UPPERCASE_SCALE = 0.9f;
    scores[IDX('a')] = 8.167f; scores[IDX('A')] = scores[IDX('a')] * UPPERCASE_SCALE;
    scores[IDX('b')] = 1.492f; scores[IDX('B')] = scores[IDX('b')] * UPPERCASE_SCALE;
    scores[IDX('c')] = 2.782f; scores[IDX('C')] = scores[IDX('c')] * UPPERCASE_SCALE;
    scores[IDX('d')] = 4.253f; scores[IDX('D')] = scores[IDX('d')] * UPPERCASE_SCALE;
    scores[IDX('e')] = 12.702f; scores[IDX('E')] = scores[IDX('e')] * UPPERCASE_SCALE;
    scores[IDX('f')] = 2.228f; scores[IDX('F')] = scores[IDX('f')] * UPPERCASE_SCALE;
    scores[IDX('g')] = 2.015f; scores[IDX('G')] = scores[IDX('g')] * UPPERCASE_SCALE;
    scores[IDX('h')] = 6.094f; scores[IDX('H')] = scores[IDX('h')] * UPPERCASE_SCALE;
    scores[IDX('i')] = 6.966f; scores[IDX('I')] = scores[IDX('i')] * UPPERCASE_SCALE;
    scores[IDX('j')] = 0.153f; scores[IDX('J')] = scores[IDX('j')] * UPPERCASE_SCALE;
    scores[IDX('k')] = 0.772f; scores[IDX('K')] = scores[IDX('k')] * UPPERCASE_SCALE;
    scores[IDX('l')] = 4.025f; scores[IDX('L')] = scores[IDX('l')] * UPPERCASE_SCALE;
    scores[IDX('m')] = 2.406f; scores[IDX('M')] = scores[IDX('m')] * UPPERCASE_SCALE;
    scores[IDX('n')] = 6.749f; scores[IDX('N')] = scores[IDX('n')] * UPPERCASE_SCALE;
    scores[IDX('o')] = 7.507f; scores[IDX('O')] = scores[IDX('o')] * UPPERCASE_SCALE;
    scores[IDX('p')] = 1.929f; scores[IDX('P')] = scores[IDX('p')] * UPPERCASE_SCALE;
    scores[IDX('q')] = 0.095f; scores[IDX('Q')] = scores[IDX('q')] * UPPERCASE_SCALE;
    scores[IDX('r')] = 5.987f; scores[IDX('R')] = scores[IDX('r')] * UPPERCASE_SCALE;
    scores[IDX('s')] = 6.327f; scores[IDX('S')] = scores[IDX('s')] * UPPERCASE_SCALE;
    scores[IDX('t')] = 9.056f; scores[IDX('T')] = scores[IDX('t')] * UPPERCASE_SCALE;
    scores[IDX('u')] = 2.758f; scores[IDX('U')] = scores[IDX('u')] * UPPERCASE_SCALE;
    scores[IDX('v')] = 0.978f; scores[IDX('V')] = scores[IDX('v')] * UPPERCASE_SCALE;
    scores[IDX('w')] = 2.361f; scores[IDX('W')] = scores[IDX('w')] * UPPERCASE_SCALE;
    scores[IDX('x')] = 0.150f; scores[IDX('X')] = scores[IDX('x')] * UPPERCASE_SCALE;
    scores[IDX('y')] = 1.974f; scores[IDX('Y')] = scores[IDX('y')] * UPPERCASE_SCALE;
    scores[IDX('z')] = 0.074f; scores[IDX('Z')] = scores[IDX('z')] * UPPERCASE_SCALE;

    scores[IDX(' ')] = 13.0f;
    scores[IDX('\n')] = 6.0f;
    scores[IDX('\'')] = 7.0f;
    scores[IDX('\"')] = 7.0f;

    float result = 0.0f;
    for(unsigned char b : bytes) {
        if (!printable(b))
            result -= 10.0f;
        else
            result += scores[b];
    }

    return result;
}
} //namespace ascii

namespace openssl {
void init() {
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(nullptr);
}

void cleanup() {
    EVP_cleanup();
    ERR_free_strings();
}

std::string decrypt(const std::string& bytes, const std::string& key, const EVP_CIPHER* mode) {
    std::string result;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx) {
        if (1 == EVP_DecryptInit_ex(ctx, mode, nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr)) {
            int decrypted_size = 0;
            unsigned char* buffer = new unsigned char[bytes.size()];
            const unsigned char* byte_ptr = reinterpret_cast<const unsigned char*>(bytes.c_str());
            
            int len = 0;
            EVP_DecryptUpdate(ctx, buffer, &len, byte_ptr, bytes.size());
            decrypted_size += len;
            EVP_DecryptFinal_ex(ctx, buffer + len, &len);
            decrypted_size += len;

            result = std::string(reinterpret_cast<char*>(buffer), decrypted_size);

            delete [] buffer;
        }

        EVP_CIPHER_CTX_free(ctx);
    }

    return result;
}
} //namespace openssl

