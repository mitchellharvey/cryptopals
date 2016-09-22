#include "Utils.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Please specify the challenege8 file!" << std::endl;
        return 1;
    }

    std::ifstream ifile(argv[1], std::ios_base::in);
    if (!ifile.is_open()) {
        std::cout << "Unable to open file: " << argv[1] << std::endl;
        return 2;
    }

    std::vector<std::string> bytes;
    for(std::string line; std::getline(ifile, line); ) {
        bytes.push_back(hex::decode(line));
    }

    ifile.close();

    // AES ECB will produce identical cipher blocks for identical inputs
    // The number of repeating 128bit blocks is the score.  The block with
    // the highest score is the winner!
    using BLOCK_SCORE = std::pair<size_t, size_t>;
    const size_t CHUNK_SIZE_BYTES = 16;
    std::vector<BLOCK_SCORE> scores;
    for(size_t i = 0; i < bytes.size(); ++i) {
        BLOCK_SCORE bs;
        bs.first = i;
        bs.second = 0;

        std::string &cur_bytes = bytes[i];
        size_t total_chunks = cur_bytes.size() / CHUNK_SIZE_BYTES;
        for(size_t chunk = 0; chunk < total_chunks; ++chunk) {
            size_t chunk_start = chunk * CHUNK_SIZE_BYTES;
            size_t search_offset = chunk_start + CHUNK_SIZE_BYTES;
            const std::string c = std::string(&cur_bytes[chunk_start], CHUNK_SIZE_BYTES);
            while(cur_bytes.find(c, search_offset) != std::string::npos) {
                bs.second++;
                search_offset += CHUNK_SIZE_BYTES;
            }
        }

        scores.push_back(bs);
    }

    std::sort(scores.begin(), scores.end(),
            [](BLOCK_SCORE bs1, BLOCK_SCORE bs2) {
                return bs1.second > bs2.second;
            }
        );

    std::cout << "Most likley AES 128bit ECB Encrypted Bytes: " << std::endl << hex::encode(bytes[scores[0].first]) << std::endl;
    std::cout << "Repeating 128bit blocks: " << scores[0].second << std::endl;
    std::cout << "Index: " << scores[0].first << std::endl;

    return 0;
}

