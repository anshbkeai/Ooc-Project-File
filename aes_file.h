#ifndef AES_FILE_H
#define AES_FILE_H
#include <iostream>
#include <fstream>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <iomanip>
#include <cstring>
#include <bits/stdc++.h>
#include "Token_Service.h"


//  function to print data in hexadecimal
// after encrypt or decrypt data, the result is typically raw binary data,
// converting each byte into a two-digit hexadecimal representation.

class  File {
    public :
    Test token_service;
   // //ger //;
void print_hex(const unsigned char *data, int length) {
    for (int i = 0; i < length; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    std::cout << std::endl;
}
std::vector<unsigned char> generateRandomBytes(int length) {
    std::vector<unsigned char> bytes(length);
    if (RAND_bytes(bytes.data(), length) != 1) {
        //.error("Error generating random bytes");
        throw std::runtime_error("Error generating random bytes");
    }
    return bytes;
}

std::string generateRandomPattern(int length) {
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string result;
    result.reserve(length);
    std::vector<unsigned char> indices = generateRandomBytes(length);
    for (int i = 0; i < length; ++i) {
        result.push_back(charset[indices[i] % charset.length()]);
    }
    return result;
}

bool encrypt_file(std::string input_file,std::string output_file, std::string token) {
    unsigned char key[AES_BLOCK_SIZE];
    if (!RAND_bytes(key, sizeof(key))) {
        //.error("Error generating random key.");
        std::cerr << "Error generating random key." << std::endl;
        return false;
    }
    std::cout << "KEY: ";
    print_hex(key, AES_BLOCK_SIZE);

    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        //.error("Error generating random IV.");
        std::cerr << "Error generating random IV." << std::endl;
        return false;
    }
    std::cout << "IV: ";
    print_hex(iv, AES_BLOCK_SIZE);

    AES_KEY encryptKey;
    AES_set_encrypt_key(key, 128, &encryptKey);

    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(output_file, std::ios::binary);

    if (!in || !out) {
        //.error("Error opening files.");
        std::cerr << "Error opening files." << std::endl;
        return false;
    }

    std::string separator = generateRandomPattern(16);
    std::cout << "Separator: " << separator << std::endl;

    // Write token, IV, separator, and key at the top of the file
    out.write(token.c_str(), token.length());
    out.write("MIT", 3);  // Token delimiter
    out.write(reinterpret_cast<const char *>(iv), AES_BLOCK_SIZE);
    out.write(separator.c_str(), separator.size());
    out.write(reinterpret_cast<const char *>(key), AES_BLOCK_SIZE);

    unsigned char buffer[AES_BLOCK_SIZE];
    unsigned char encrypted_buffer[AES_BLOCK_SIZE];
    int bytes_read;

    while ((bytes_read = in.readsome(reinterpret_cast<char *>(buffer), AES_BLOCK_SIZE)) > 0) {
        if (bytes_read < AES_BLOCK_SIZE) {
            int padding = AES_BLOCK_SIZE - bytes_read;
            memset(buffer + bytes_read, padding, padding);
            bytes_read = AES_BLOCK_SIZE;
        }

        AES_cbc_encrypt(buffer, encrypted_buffer, bytes_read, &encryptKey, iv, AES_ENCRYPT);
        out.write(reinterpret_cast<const char *>(encrypted_buffer), bytes_read);
    }
    
    in.close();
    out.close();
    return true;
}

    // Extract token
    // std::string token;
    // char ch;
    // while (in.get(ch) && ch != 'M') {
    //     token += ch;
    // }
    // if (ch == 'M') {
    //     if (in.get(ch) && ch == 'I' && in.get(ch) && ch == 'T') {
    //         std::cout << "Extracted Token: " << token << std::endl;
    //     } else {
    //         std::cerr << "Error: Invalid token format LUND   MERA ." << std::endl;
    //         return false;
    //     }
    // } else {
    //     std::cerr << "Error: Token not found." << std::endl;
    //     return false;
    // }
bool decrypt_file(std::string input_file, std::string output_file, std::string sender) {
    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(output_file, std::ios::binary);

    if (!in || !out) {
        //.error("Error opening files.");
        std::cerr << "Error opening files." << std::endl;
        return false;
    }

    // Read until MIT marker
    std::vector<unsigned char> content;
    unsigned char byte;
    std::string mitMarker;
    bool foundMIT = false;

    while (in.read(reinterpret_cast<char*>(&byte), 1)) {
        content.push_back(byte);
        
        // Check for MIT marker
        if (byte == 'M' || byte == 'I' || byte == 'T') {
            mitMarker += byte;
            if (mitMarker == "MIT") {
                foundMIT = true;
                // Remove "MIT" from content
                content.resize(content.size() - 3);
                break;
            }
        } else {
            mitMarker.clear();
        }
    }

    if (!foundMIT) {
        //.error( "MIT marker not found" );
        std::cerr << "MIT marker not found" << std::endl;
        return false;
    }

    // Convert content to string for token extraction
    std::string token(content.begin(), content.end());
    
    //std::cout << "Token found: " << token << std::endl;
    
    // Process the token
    string decrypt_token;
    try {
        decrypt_token = token_service.decrypt_token(token);
        cout << "Decrypted token: " << decrypt_token << endl;
    } catch (const std::exception& e) {
        std::cerr << "Token decryption failed: " << e.what() << std::endl;
        return false;
    }

    if (!token_service.validate_Token(decrypt_token, sender)) {
        //.error("Token validation failed");
        cout << "Token validation failed" << endl;
        return false;
    }

    // Read IV (16 bytes)
    unsigned char iv[AES_BLOCK_SIZE];
    if (!in.read(reinterpret_cast<char*>(iv), AES_BLOCK_SIZE)) {
        //.error( "Failed to read IV");
        std::cerr << "Failed to read IV" << std::endl;
        return false;
    }
    std::cout << "IV: ";
    print_hex(iv, AES_BLOCK_SIZE);

    // Read separator (16 bytes)
    char separator[17];
    if (!in.read(separator, 16)) {
        std::cerr << "Failed to read separator" << std::endl;
        return false;
    }
    separator[16] = '\0';
    //std::cout << "Separator: " << separator << std::endl;

    // Read key (16 bytes)
    unsigned char key[AES_BLOCK_SIZE];
    if (!in.read(reinterpret_cast<char*>(key), AES_BLOCK_SIZE)) {
        std::cerr << "Failed to read key" << std::endl;
        return false;
    }
    std::cout << "Key: ";
    print_hex(key, AES_BLOCK_SIZE);

    // Setup decryption
    AES_KEY aesKey;
    if (AES_set_decrypt_key(key, 128, &aesKey) < 0) {
        std::cerr << "Failed to set decryption key" << std::endl;
        return false;
    }

    // Read and decrypt the rest of the file in blocks
    unsigned char inBuf[AES_BLOCK_SIZE];
    unsigned char outBuf[AES_BLOCK_SIZE];
    unsigned char ivCopy[AES_BLOCK_SIZE];
    memcpy(ivCopy, iv, AES_BLOCK_SIZE);

    // Create a buffer for the decrypted data
    std::vector<unsigned char> decrypted;

    while (in.read(reinterpret_cast<char*>(inBuf), AES_BLOCK_SIZE)) {
        AES_cbc_encrypt(inBuf, outBuf, AES_BLOCK_SIZE, &aesKey, ivCopy, AES_DECRYPT);
        
        // If this is the last block, handle padding
        if (in.eof()) {
            int paddingLen = outBuf[AES_BLOCK_SIZE - 1];
            if (paddingLen > 0 && paddingLen <= AES_BLOCK_SIZE) {
                decrypted.insert(decrypted.end(), outBuf, outBuf + (AES_BLOCK_SIZE - paddingLen));
            }
        } else {
            decrypted.insert(decrypted.end(), outBuf, outBuf + AES_BLOCK_SIZE);
        }
    }

    // Write the decrypted data
    out.write(reinterpret_cast<const char*>(decrypted.data()), decrypted.size());

    in.close();
    out.close();
    return true;
}
};
// int main() {
//     // Generate a random AES key
    

//     const char *input_file = "input.txt";
//     const char *encrypted_file = "encrypted.bin";
//     const char *decrypted_file = "decrypted.txt";
//     // have to add token  pass
//     std::string token  = "vedansh123";
//     // Encrypt the file
//     if (encrypt_file(input_file, encrypted_file, token)) {
//         std::cout << "File encrypted successfully!" << std::endl;
//     } else {
//         std::cerr << "File encryption failed." << std::endl;
//         return 1;
//     }

//     // Decrypt the file
//     if (decrypt_file(encrypted_file, decrypted_file,"user")) {
//         std::cout << "File decrypted successfully!" << std::endl;
//     } else {
//         std::cerr << "File decryption failed." << std::endl;
//         return 1;
//     }

//     return 0;
// }
#endif