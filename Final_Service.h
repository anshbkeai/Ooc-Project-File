#ifndef FINAL_SERVICE_H
#define FINAL_SERVICE_H

#include <iostream>
#include <string>
#include "Token.h"
#include "Timer.h"
#include "aes_file.h"
#include "Token_service.h"
#include "Logger.h"

class Service {
private:
    File file;
    Test token_service;
    Timer time;
    Logger logger;  // Logger member variable

public:
    // Constructor to initialize the logger
    Service() : logger("service.log", 1024 * 1024) {
        logger.info("Service initialized");
    }

    // Encrypt function
    bool Encrypt(std::string user, std::string receiver, std::string input_file_path, 
                std::string expiration_time, std::string output_file_path) {
        try {
            logger.info("Starting encryption process");
            logger.info("User: " + user + ", Receiver: " + receiver);
            logger.info("Input file: " + input_file_path);
            logger.info("Output file: " + output_file_path);

            Token token(user, receiver, time.current_Time(), expiration_time, ".txt");
            logger.info("Token created with expiration time: " + expiration_time);

            string encrypted_token = token_service.genrate_Token(token);
            logger.info("Token generated successfully");

            bool result = file.encrypt_file(input_file_path, output_file_path, encrypted_token);
            
            if (result) {
                logger.info("File encryption completed successfully");
            } else {
                logger.error("File encryption failed");
            }

            return result;
        }
        catch (const std::exception& e) {
            logger.error("Encryption error: " + std::string(e.what()));
            return false;
        }
        catch (...) {
            logger.error("Unknown error occurred during encryption");
            return false;
        }
    }

    // Decrypt function
    bool Decrypt(std::string sender, std::string input_file_path, 
                std::string output_file_path) {
        try {
            logger.info("Starting decryption process");
            logger.info("Sender: " + sender);
            logger.info("Input file: " + input_file_path);
            logger.info("Output file: " + output_file_path);

            bool result = file.decrypt_file(input_file_path, output_file_path, sender);
            
            if (result) {
                logger.info("File decryption completed successfully");
            } else {
                logger.error("File decryption failed");
            }

            return result;
        }
        catch (const std::exception& e) {
            logger.error("Decryption error: " + std::string(e.what()));
            return false;
        }
        catch (...) {
            logger.error("Unknown error occurred during decryption");
            return false;
        }
    }

    ~Service() {
        logger.info("Service shutting down");
    }
};

#endif // FINAL_SERVICE_H