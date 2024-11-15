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
#include "Logger.h"

class File {
public:
    Test token_service;
    Logger logger; // Logger member variable

    // Constructor to initialize the logger
    File() : logger("file.log", 1024 * 1024) {
        logger.info("File class initialized");
    }

    void print_hex(const unsigned char *data, int length) {
        for (int i = 0; i < length; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
        }
        std::cout << std::endl;
    }

    std::vector<unsigned char> generateRandomBytes(int length) {
        std::vector<unsigned char> bytes(length);
        if (RAND_bytes(bytes.data(), length) != 1) {
            logger.error("Error generating random bytes");
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
        logger.info("Random pattern generated");
        return result;
    }

    bool encrypt_file(std::string input_file, std::string output_file, std::string token) {
        logger.info("Starting file encryption");
        unsigned char key[AES_BLOCK_SIZE];
        if (!RAND_bytes(key, sizeof(key))) {
            logger.error("Error generating random key");
            return false;
        }
        logger.info("Random key generated");

        unsigned char iv[AES_BLOCK_SIZE];
        if (!RAND_bytes(iv, sizeof(iv))) {
            logger.error("Error generating random IV");
            return false;
        }
        logger.info("Random IV generated");

        AES_KEY encryptKey;
        AES_set_encrypt_key(key, 128, &encryptKey);

        std::ifstream in(input_file, std::ios::binary);
        std::ofstream out(output_file, std::ios::binary);

        if (!in || !out) {
            logger.error("Error opening files for encryption");
            return false;
        }

        std::string separator = generateRandomPattern(16);
        logger.info("Separator generated");

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
        logger.info("File encryption completed");
        return true;
    }

    bool decrypt_file(std::string input_file, std::string output_file, std::string sender) {
        logger.info("Starting file decryption");

        std::ifstream in(input_file, std::ios::binary);
        std::ofstream out(output_file, std::ios::binary);

        if (!in || !out) {
            logger.error("Error opening files for decryption");
            return false;
        }

        std::vector<unsigned char> content;
        unsigned char byte;
        std::string mitMarker;
        bool foundMIT = false;

        while (in.read(reinterpret_cast<char*>(&byte), 1)) {
            content.push_back(byte);

            if (byte == 'M' || byte == 'I' || byte == 'T') {
                mitMarker += byte;
                if (mitMarker == "MIT") {
                    foundMIT = true;
                    content.resize(content.size() - 3);
                    break;
                }
            } else {
                mitMarker.clear();
            }
        }

        if (!foundMIT) {
            logger.error("MIT marker not found");
            return false;
        }

        std::string token(content.begin(), content.end());
        std::string decrypt_token;

        try {
            decrypt_token = token_service.decrypt_token(token);
            logger.info("Token decrypted successfully");
        } catch (const std::exception& e) {
            logger.error("Token decryption failed: " + std::string(e.what()));
            return false;
        }

        if (!token_service.validate_Token(decrypt_token, sender)) {
            logger.error("Token validation failed");
            return false;
        }

        unsigned char iv[AES_BLOCK_SIZE];
        if (!in.read(reinterpret_cast<char*>(iv), AES_BLOCK_SIZE)) {
            logger.error("Failed to read IV");
            return false;
        }

        char separator[17];
        if (!in.read(separator, 16)) {
            logger.error("Failed to read separator");
            return false;
        }
        separator[16] = '\0';

        unsigned char key[AES_BLOCK_SIZE];
        if (!in.read(reinterpret_cast<char*>(key), AES_BLOCK_SIZE)) {
            logger.error("Failed to read key");
            return false;
        }

        AES_KEY aesKey;
        if (AES_set_decrypt_key(key, 128, &aesKey) < 0) {
            logger.error("Failed to set decryption key");
            return false;
        }

        unsigned char inBuf[AES_BLOCK_SIZE];
        unsigned char outBuf[AES_BLOCK_SIZE];
        unsigned char ivCopy[AES_BLOCK_SIZE];
        memcpy(ivCopy, iv, AES_BLOCK_SIZE);

        std::vector<unsigned char> decrypted;

        while (in.read(reinterpret_cast<char*>(inBuf), AES_BLOCK_SIZE)) {
            AES_cbc_encrypt(inBuf, outBuf, AES_BLOCK_SIZE, &aesKey, ivCopy, AES_DECRYPT);
            if (in.eof()) {
                int paddingLen = outBuf[AES_BLOCK_SIZE - 1];
                if (paddingLen > 0 && paddingLen <= AES_BLOCK_SIZE) {
                    decrypted.insert(decrypted.end(), outBuf, outBuf + (AES_BLOCK_SIZE - paddingLen));
                }
            } else {
                decrypted.insert(decrypted.end(), outBuf, outBuf + AES_BLOCK_SIZE);
            }
        }

        out.write(reinterpret_cast<const char*>(decrypted.data()), decrypted.size());

        in.close();
        out.close();
        logger.info("File decryption completed");
        return true;
    }
};

#endif
