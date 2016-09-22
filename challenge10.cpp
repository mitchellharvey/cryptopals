#include "Utils.h"

#include <iostream>

std::string cbc_encrypt(const std::string& bytes, const std::string& key, const std::string& iv) {
    // Implement CBC encryption manually, xor input block with last encrypted block
    // then encrypt that.  First block is xor'd with the iv
    std::string result;
    std::string last_xor = iv;
    
    const size_t BLOCK_SIZE = 16;
    std::string padded = cipher::pad_pkcs7(bytes, BLOCK_SIZE);
    for(size_t i = 0; i < padded.size(); i += BLOCK_SIZE) {
        std::string block(&padded[i], BLOCK_SIZE);
        last_xor = openssl::encrypt(/*cipher::repeating_xor(block, last_xor)*/block, key, EVP_aes_128_ecb());
        result += last_xor;
    }

    return result;

    /*
    // Implement CBC encryption manually, xor input block with last encrypted block
    // then encrypt that.  First block is xor'd with the iv
    std::string result;
    std::string last_xor = iv;
    
    const size_t BLOCK_SIZE = 16;
    std::string padded = cipher::pad_pkcs7(bytes, BLOCK_SIZE);
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx) {
        if (1 == EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr)) {
            int len = 0;
            unsigned char buffer[BLOCK_SIZE] = {0};
            for(size_t i = 0; i < padded.size(); i += BLOCK_SIZE) {
                std::string block(&padded[i], BLOCK_SIZE);

                std::string xord_block = cipher::repeating_xor(block, last_xor);
                EVP_EncryptUpdate(ctx, buffer, &len,
                        reinterpret_cast<const unsigned char*>(xord_block.c_str()), xord_block.size());
                last_xor = std::string(reinterpret_cast<char *>(buffer), len);
                result += last_xor;
            }

            EVP_EncryptFinal_ex(ctx, buffer, &len);
            result += std::string(reinterpret_cast<char *>(buffer), len);
        }

        EVP_CIPHER_CTX_free(ctx);
    }

    return result;
    */
}

std::string cbc_decrypt(const std::string& bytes, const std::string& key, const std::string& iv) {

    std::string result;
    std::string last_xor = iv;

    const size_t BLOCK_SIZE = 16;
    for(size_t i = 0; i < bytes.size(); i += BLOCK_SIZE) {
        std::string encrypted(bytes.c_str() + i, BLOCK_SIZE);
        std::string decrypted = openssl::decrypt(encrypted, key, EVP_aes_128_ecb());
        result += cipher::repeating_xor(decrypted, last_xor);
        last_xor = encrypted;
    }

    //return openssl::decrypt(bytes, key, EVP_aes_128_ecb());
    return result;

    /*
    std::string result;
    std::string last_xor = iv;
    
    const size_t BLOCK_SIZE = 16;
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx) {
        if (1 == EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr)) {
            int len = 0;
            //unsigned char buffer[BLOCK_SIZE] = {0};
            unsigned char *buffer = new unsigned char[BLOCK_SIZE * 2];
            for(size_t i = 0; i < bytes.size(); i += BLOCK_SIZE) {
                std::string block(&bytes[i], BLOCK_SIZE);

                EVP_DecryptUpdate(ctx, buffer, &len,
                        reinterpret_cast<const unsigned char*>(block.c_str()), block.size());

                if (len) {
                    result += cipher::repeating_xor(
                            std::string(reinterpret_cast<char *>(buffer), len), last_xor);
                    // last_xor = std::string(reinterpret_cast<char *>(buffer), len);

                    last_xor = block;
                }
            }

            EVP_DecryptFinal_ex(ctx, buffer, &len);
            if (len) {
                result += cipher::repeating_xor(
                            std::string(reinterpret_cast<char *>(buffer), len), last_xor);
            }

            delete [] buffer;
        }

        EVP_CIPHER_CTX_free(ctx);
    }

    return result;
    */
}

int main(int argc, char** argv) {

    std::string input = "YELLOW SUBMARINEYELLOW SUBMARINE";
    std::string key = "MyKey";
    std::string iv = "MyInitializationVector";

    openssl::init();

    std::string encrypted = cbc_encrypt(input, key, iv);
    std::cout << "Input: " << input << std::endl;
    std::cout << "Key: " << key << std::endl;
    std::cout << "IV: " << iv << std::endl;

    std::string encrypted_e;
    std::cout << "ECB Encrypted: " << (encrypted_e = openssl::encrypt(input, key, EVP_aes_128_ecb())) << std::endl;
    std::cout << "ECB Decrypted: " << openssl::decrypt(encrypted_e, key, EVP_aes_128_ecb()) << std::endl;

    std::cout << "CBC Encrypted: " << hex::encode(encrypted) << std::endl;
    std::cout << "CBC Decrypted: " << cbc_decrypt(encrypted, key, iv) << std::endl;

    openssl::cleanup();

    return 0;
}

