#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <string>
#include <chrono>
#include <stdexcept>

class Timer {
public:
    // Function to convert date string to time_t
    std::time_t convertToTimeT(const std::string& dateStr) {
        std::tm tm = {};
        std::istringstream ss(dateStr);
        
        // Use std::get_time to parse the date string
        ss >> std::get_time(&tm, "%m %d %H:%M:%S %Y");
        
        if (ss.fail()) {
            throw std::runtime_error("Failed to parse date string");
        }

        return std::mktime(&tm); // Convert tm to time_t
    }

    // Function to compare two date strings
    bool compareDates(const std::string& expiration_time) {
        std::time_t e_time = convertToTimeT(expiration_time);
        auto now = std::chrono::system_clock::now();
    
        // Convert it to time_t to output a human-readable time
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    
        return (e_time >= currentTime);
    }

    // Function to get the current time in the specified format
    std::string current_Time() {
        auto now = std::chrono::system_clock::now();
        
        // Convert time_point to time_t
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        
        // Convert time_t to tm structure
        std::tm* tmPtr = std::localtime(&currentTime);
        
        // Format the time to the desired format
        std::ostringstream oss;
        oss << std::put_time(tmPtr, "%m %d %H:%M:%S %Y");
        
        return oss.str();
    }
};

#endif // TIMER_H