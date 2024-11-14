#ifndef TIME_H 
#define TIME_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>


using  namespace  std;

// Function to get formatted expiration time from user


class  e_time {

    public:
        string formatExpirationTime() {
    // Create a tm structure
                int month, day, year, hour, minute, second;

    // Collect expiration date and time from user
    cout << "Enter expiration date and time:\n";
    cout << "Month (1-12): ";
    cin >> month;
    cout << "Day (1-31): ";
    cin >> day;
    cout << "Year (e.g., 2024): ";
    cin >> year;
    cout << "Hour (0-23): ";
    cin >> hour;
    cout << "Minute (0-59): ";
    cin >> minute;
    cout << "Second (0-59): ";
    cin >> second;

    // Create a tm structure
    tm expiration_tm = {};
    expiration_tm.tm_year = year - 1900; // Years since 1900
    expiration_tm.tm_mon = month - 1;    // Months since January
    expiration_tm.tm_mday = day;
    expiration_tm.tm_hour = hour;
    expiration_tm.tm_min = minute;
    expiration_tm.tm_sec = second;
    expiration_tm.tm_isdst = -1; // Not considering daylight saving time

    // Create a buffer to hold the formatted time string
    char buffer[20]; // Buffer size should be enough for "mm dd HH:MM:SS YYYY\0"
    
    // Format the time using strftime
    strftime(buffer, sizeof(buffer), "%m %d %H:%M:%S %Y", &expiration_tm);

    return string(buffer);
        }

};
#endif