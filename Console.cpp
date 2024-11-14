#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "windows.h"
#include "User.h"
#include "Token.h"
#include "Timer.h"
#include "Vector.h"
#include "Time_Convert.h"
#include "Logger.h"
#include "Final_Service.h"

using namespace std;

void loadingBar(int seconds)
{
    const int total = 20;
    for (int i = 0; i <= total; ++i)
    {
        int progress = (i * 100) / total;
        cout << "\r[";
        int pos = total * progress / 100;
        for (int j = 0; j < total; ++j)
        {
            if (j < pos)
                cout << "=";
            else
                cout << " ";
        }
        cout << "] " << progress << "%";
        cout.flush();
        Sleep(seconds * 1000 / total);
    }
    cout << endl;
}

int main()
{
    // Initialize logger first, before any other operations
    Logger logger("app.log", 1024 * 1024); // 1MB size limit

    try
    {
        db Repo;
        Service service;
        e_time exp_time;

        logger.info("Application started");

        cout << "===============================" << endl;
        cout << "  Hello User! Welcome to the App  " << endl;
        cout << "===============================" << endl;

        string username, pass;
        cout << "\nPlease enter your Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> pass;

        User user(username, pass);
        logger.info("Login attempt for user: " + username);

        if (Repo.search(user))
        {
            logger.info("User " + username + " successfully logged in");
            cout << "\nWelcome, " << user.get_username() << "!" << endl;

            int choice = 0;
            do
            {
                cout << "\nPlease select an option:" << endl;
                cout << "1. File Encryption" << endl;
                cout << "2. File Decryption" << endl;
                cout << "3. Exit" << endl;
                cout << "Your choice: ";
                cin >> choice;

                switch (choice)
                {
                case 1:
                {
                    logger.info("User selected file encryption");
                    cout << "\n-- File Encryption Service --" << endl;
                    string receiver, expiration_time, input_file_path;

                    cout << "Enter the Receiver's Username: ";
                    cin >> receiver;

                    if (!Repo.findByUsername(receiver))
                    {
                        logger.warn("Invalid receiver username entered: " + receiver);
                        cout << "\nError: Receiver does not exist in the database." << endl;
                        cout << "Do you want to try again or exit? (1 for Retry, 2 for Exit): ";
                        int retryChoice;
                        cin >> retryChoice;
                        if (retryChoice == 2)
                        {
                            break;
                        }
                        else
                        {
                            continue;
                        }
                    }

                    expiration_time = exp_time.formatExpirationTime();
                    cout << "TIME IS  " << expiration_time << endl;

                    cout << "Enter the file location: ";
                    cin >> input_file_path;

                    logger.info("Processing encryption request - File: " + input_file_path +
                                ", Receiver: " + receiver);

                    cout << "Hey, tell me where you want the output file saved: ";
                    string output_file;
                    cin >> output_file;

                    loadingBar(5);

                    try
                    {
                        service.Encrypt(username, receiver, input_file_path, expiration_time, output_file);
                        logger.info("File encryption successful - Output: " + output_file);
                        cout << "File has been successfully encrypted!" << endl;
                    }
                    catch (const std::exception &e)
                    {
                        logger.error("Encryption failed: " + string(e.what()));
                        cout << "Error during encryption: " << e.what() << endl;
                    }
                    break;
                }
                case 2:
                {
                    logger.info("User selected file decryption");

                    cout << "\n-- File Decryption Service --" << endl;
                    string input_file_path;
                    cout << "Enter the file location: ";
                    cin >> input_file_path;
                    string sender;
                    cout << "Enter the Sender's Username: ";
                    cin >> sender;
                    if (!Repo.findByUsername(sender))
                    {
                        logger.warn("Invalid sender username entered: " + sender);
                        cout << "\nError: sender does not exist in the database." << endl;
                        cout << "Do you want to try again or exit? (1 for Retry, 2 for Exit): ";
                        int retryChoice;
                        cin >> retryChoice;
                        if (retryChoice == 2)
                        {
                            break;
                        }
                        else
                        {
                            continue;
                        }
                    }

                    cout << "Hey, tell me where you want the output file saved: ";
                    string output_file;
                    cin >> output_file;

                    logger.info("Processing decryption request - File: " + input_file_path);

                    loadingBar(5);

                    try
                    {
                        service.Decrypt(sender, input_file_path, output_file);
                        logger.info("File decryption successful - Output: " + output_file);
                        cout << "File has been successfully decrypted!" << endl;
                    }
                    catch (const std::exception &e)
                    {
                        logger.error("Decryption failed: " + string(e.what()));
                        cout << "Error during decryption: " << e.what() << endl;
                    }
                    break;
                }
                case 3:
                    logger.info("User " + username + " logging out");
                    cout << "Exiting the application. Thank you for using the app!" << endl;
                    break;
                default:
                    logger.warn("Invalid menu choice selected: " + to_string(choice));
                    cout << "\nInvalid choice. Please select a valid option." << endl;
                }
            } while (choice != 3);
        }
        else
        {
            logger.warn("Failed login attempt for user: " + username);
            cout << "\nError: Invalid username or password!" << endl;
        }
    }
    catch (const std::exception &e)
    {
        logger.error("Application error: " + string(e.what()));
        cout << "An error occurred: " << e.what() << endl;
    }

    logger.info("Application shutting down");
    return 0;
}