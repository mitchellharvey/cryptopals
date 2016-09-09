#include "Utils.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Please specify the challenege6 file!" << std::endl;
        return 1;
    }

    std::ifstream ifile(argv[1], std::ios_base::in);
    if (!ifile.is_open()) {
        std::cout << "Unable to open file: " << argv[1] << std::endl;
        return 2;
    }

    // Load each line, base64 decode it and append it to the list of
    // bytes to operate on
    std::string bytes;
    for(std::string line; std::getline(ifile, line); ) {
        bytes += base64::decode(line);
    }

    ifile.close();

    // Attempt to guess the repeating xor key size by comparing the edit distance
    // between each key_size chunk of bytes and normalizing it (dividing the edit distance
    // result by the current key_size).  The key size with the smallest distance is most
    // likey the correct size of the repeating xor key
    size_t min_key_size = 2;
    size_t max_key_size = 40;
    using KEY_SIZE = std::pair<size_t, double>;
    std::vector<KEY_SIZE> sorted_key_sizes;
    for(size_t key_size = min_key_size; key_size <= max_key_size; ++key_size) {

        size_t total_blocks = bytes.size() / key_size;
        double normalized_distance = 0.0f;
        for(size_t b = 0; b < total_blocks; b+=2) {
            const char* b1_start = bytes.c_str() + (b * key_size);
            const char* b2_start = bytes.c_str() + (b+1 * key_size);
            std::string block1(b1_start, std::min(key_size, bytes.size()));
            std::string block2(b2_start, std::min(key_size, bytes.size() - key_size));
            normalized_distance += (static_cast<double>(cipher::hamming_distance(block1, block2)) / key_size);
        }
        normalized_distance /= total_blocks;
        sorted_key_sizes.push_back(KEY_SIZE(key_size, normalized_distance));
    }

    // Sort key sizes from lowest to highest
    std::sort(sorted_key_sizes.begin(), sorted_key_sizes.end(), 
                [](const KEY_SIZE& p1, const KEY_SIZE& p2) {
                    return p1.second < p2.second;
                }
            );

    // Starting with the most likley key size, break the bytes up into key_size number of blocks
    // and for each block, fill it with all the bytes that would be encrypted with the 
    // same xor byte.
    for(auto key_size : sorted_key_sizes) {
        size_t cur_key_size = key_size.first;
        std::vector<std::string> blocks(cur_key_size);
        for(size_t i = 0; i < blocks.size(); ++i) {
            const char* cur = bytes.c_str() + i;
            const char* end = bytes.c_str() + bytes.size();
            while(cur <= end) {
                blocks[i] += *cur;
                cur += cur_key_size;
            }
        }

        // Now we have our blocks, guess the single xor char for each block
        std::string repeating_xor_key;
        for(auto block : blocks) {
            repeating_xor_key += cipher::guess_xor_byte(block);
        }

        std::cout << "Key Size: " << cur_key_size << std::endl;
        std::cout << "Score: " << key_size.second << std::endl;
        std::cout << "Key: " << hex::encode(repeating_xor_key) << std::endl;

        std::cout << cipher::repeating_xor(bytes, repeating_xor_key) << std::endl;

        break;
    }

    return 0;
}


