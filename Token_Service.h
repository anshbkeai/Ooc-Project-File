#ifndef TOKEN_SERVICE_H
#define TOKEN_SERVICE_H

#include "Token.h"
#include "Timer.h"
#include "aes.h"
#include "Logger.h"
#include <iostream>
#include <string>

class Test {
public:
    Timer time;
    Aes_Token aes_token;
    Logger logger;

    // Constructor initializes the logger
    Test() : logger("token_service.log", 1024 * 1024) {
        logger.info("Token service initialized");
    }

    // Generates a token
    std::string genrate_Token(Token token) {
        logger.info("Starting token generation");

        std::string username_reciver = token.sender + "." + token.reciver;
        std::string time = token.issue_time + "." + token.expiration_time;
        std::string final_token = username_reciver + "." + time;

        // Generating key and IV
        std::string key = aes_token.generateRandomBytes(16); // 16 bytes for AES-128
        std::string iv = aes_token.generateRandomBytes(16);  // 16 bytes IV

        logger.info("Generated encryption key and IV");

        // Encrypt the final token
        std::string e_token = aes_token.encrypt(final_token, key, iv);
        logger.info("Token encrypted successfully");

        // Return formatted token
        return "kothrud" + e_token + "mit" + key + "wpu" + iv;
    }

    // Decrypts a token
    std::string decrypt_token(std::string input) {
        logger.info("Starting token decryption");

        const std::string kothrud = "kothrud";
        const std::string mit = "mit";
        const std::string wpu = "wpu";

        size_t tokenStart = input.find(kothrud) + kothrud.length();
        size_t tokenEnd = input.find(mit, tokenStart);
        size_t keyStart = tokenEnd + mit.length();
        size_t keyEnd = input.find(wpu, keyStart);

        std::string e_token = input.substr(tokenStart, tokenEnd - tokenStart);
        std::string key = input.substr(keyStart, keyEnd - keyStart);
        std::string iv = input.substr(keyEnd + wpu.length());

        logger.info("Extracted encrypted token, key, and IV for decryption");

        try {
            std::string t = aes_token.decrypt(e_token, key, iv);
            logger.info("Token decrypted successfully");
            return t;
        } catch (const std::exception& e) {
            logger.error("Token decryption failed: " + std::string(e.what()));
            throw;
        }
    }

    // Validates a token against a given user
    bool validate_Token(std::string token, std::string user1) {
        logger.info("Starting token validation");

        size_t first_dot = token.find('.');
        size_t second_dot = token.find('.', first_dot + 1);
        size_t last_dot = token.rfind('.');

        // Extract user, receiver, and time fields
        std::string user = token.substr(0, first_dot);
        std::string receiver = token.substr(first_dot + 1, second_dot - first_dot - 1);
        std::string time1 = token.substr(second_dot + 1, last_dot - second_dot - 1);
        std::string exp_time2 = token.substr(last_dot + 1);

        if (user != user1) {
            logger.error("Token validation failed: Incorrect sender");
            return false;
        }

        if (!time.compareDates(exp_time2)) {
            logger.error("Token validation failed: Token expired");
            return false;
        }

        logger.info("Token validated successfully");
        return true;
    }
};

#endif
