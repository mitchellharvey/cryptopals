#include "Utils.h"

#include <iostream>

int main(int argc, char** argv) {

    std::string cipher1 = "1c0111001f010100061a024b53535009181c";
    std::string cipher2 = "686974207468652062756c6c277320657965";

    std::cout << "Fixed XOR of [" << cipher1 << "] ^ [" << cipher2 << "]" << std::endl;

    std::string result = cipher::fixed_xor(hex::decode(cipher1), hex::decode(cipher2));
    std::cout <<  hex::encode(result) << std::endl;

    return 0;
}

