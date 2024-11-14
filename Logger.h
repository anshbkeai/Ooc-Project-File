#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "Timer.h"

namespace fs = std::filesystem;  // Add this namespace alias

class Logger {
public:
    enum class Level { INFO, WARN, Error };
    
    Logger(const std::string& filename = "log.txt", long maxSize = 1048576) 
        : log_filename(filename), max_size(maxSize) {
        rotateIfNeeded();
        openLogFile();
        info("Logger initialized");
    }

    ~Logger() {
        if (log_file.is_open()) {
            info("Logger shutting down");
            log_file.close();
        }
    }

    void log(const std::string& message, Level level) {
        if (shouldRotate()) {
            rotateLog();
        }

        std::string log_level_str;
        switch (level) {
            case Level::INFO:  log_level_str = "[INFO]"; break;
            case Level::WARN:  log_level_str = "[WARN]"; break;
            case Level::Error: log_level_str = "[ERROR]"; break;
        }

        std::string timestamp = timer.current_Time();
        std::string log_message = "[" + timestamp + "] " + log_level_str + " " + message + "\n";
        
        std::cout << log_message;
        
        if (log_file.is_open()) {
            log_file << log_message;
            log_file.flush();
        }
    }

    void info(const std::string& message) { log(message, Level::INFO); }
    void warn(const std::string& message) { log(message, Level::WARN); }
    void error(const std::string& message) { log(message, Level::Error); }

    void showLogs() const {
        std::ifstream file(log_filename);
        if (!file.is_open()) {
            std::cerr << "Unable to open log file for reading" << std::endl;
            return;
        }

        std::cout << "\n=== Log Contents ===\n" << std::endl;
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
        std::cout << "\n=== End of Logs ===\n" << std::endl;
        file.close();
    }

private:
    std::fstream log_file;
    Timer timer;
    const long max_size;
    std::string log_filename;
    const std::string backup_extension = ".old";

    void openLogFile() {
        log_file.open(log_filename, std::ios::app);
        if (!log_file.is_open()) {
            std::cerr << "Failed to open log file: " << log_filename << std::endl;
        }
    }

    long getFileSize() const {
        std::error_code ec;
        if (fs::exists(log_filename)) {
            return fs::file_size(log_filename, ec);
        }
        return 0;
    }

    bool shouldRotate() const {
        return getFileSize() >= max_size;
    }

    void rotateIfNeeded() {
        if (shouldRotate()) {
            rotateLog();
        }
    }

    void rotateLog() {
        if (log_file.is_open()) {
            log_file.close();
        }

        std::string backup_file = log_filename + backup_extension;
        
        // Remove old backup if exists
        if (fs::exists(backup_file)) {
            fs::remove(backup_file);
        }
        
        // Rename current log to backup
        if (fs::exists(log_filename)) {
            fs::rename(log_filename, backup_file);
        }
        
        // Open new log file
        openLogFile();
    }
};

#endif