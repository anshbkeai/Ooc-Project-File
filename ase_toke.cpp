#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <iostream>

// Function to handle OpenSSL errors
void handleErrors() {
    ERR_print_errors_fp(stderr);
    throw std::runtime_error("OpenSSL error");
}

std::string generateRandomBytes(size_t length) {
    unsigned char buffer[length];
    if (RAND_bytes(buffer, sizeof(buffer)) != 1) {
        throw std::runtime_error("Failed to generate random bytes");
    }
    return std::string(reinterpret_cast<char*>(buffer), length);
}

// Encryption function
std::string encrypt(const std::string& plaintext, const std::string& key, const std::string& iv) {
    EVP_CIPHER_CTX *ctx;
    
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
    
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, 
                               reinterpret_cast<const unsigned char*>(key.c_str()), 
                               reinterpret_cast<const unsigned char*>(iv.c_str())))
        handleErrors();
    
    std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
    int len;
    
    if(1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, 
                              reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size()))
        handleErrors();
    
    int ciphertext_len = len;
    
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) handleErrors();
    ciphertext_len += len;
    
    EVP_CIPHER_CTX_free(ctx);
    
    return std::string(reinterpret_cast<char*>(ciphertext.data()), ciphertext_len);
}

// Decryption function
std::string decrypt(const std::string& ciphertext, const std::string& key, const std::string& iv) {
    EVP_CIPHER_CTX *ctx;
    
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
    
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, 
                               reinterpret_cast<const unsigned char*>(key.c_str()), 
                               reinterpret_cast<const unsigned char*>(iv.c_str())))
        handleErrors();
    
    std::vector<unsigned char> plaintext(ciphertext.size());
    int len;
    
    if(1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, 
                              reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size()))
        handleErrors();
    
    int plaintext_len = len;
    
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) handleErrors();
    plaintext_len += len;
    
    EVP_CIPHER_CTX_free(ctx);
    
    return std::string(reinterpret_cast<char*>(plaintext.data()), plaintext_len);
}

// Example usage
int main() {
    std::string username_receiver = "Gaurav Kumar";
    // std::string key = "0123456789abcdef"; // 16 bytes for AES-128
    // std::string iv = "abcdefghijklmnop"; // 16 bytes for AES block size

    
    std::string en_username = encrypt(username_receiver, key, iv);
    std::string de_username = decrypt(en_username, key, iv);

    std::cout << "Original: " << username_receiver << std::endl;
    std::cout << "Encrypted: " << en_username << std::endl;
    std::cout << "Decrypted: " << de_username << std::endl;

    return 0;
}
