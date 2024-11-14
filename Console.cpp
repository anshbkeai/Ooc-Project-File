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


//#include "Global_Exception_Handler.h"
#include "Final_Service.h"

using namespace std;

void loadingBar(int seconds) {
    const int total = 20; // Total steps in the loading bar
    for (int i = 0; i <= total; ++i) {
        // Calculate the percentage of completion
        int progress = (i * 100) / total;

        // Print the loading bar
        cout << "\r[";
        int pos = total * progress / 100;
        for (int j = 0; j < total; ++j) {
            if (j < pos) cout << "=";
            else cout << " ";
        }
        cout << "] " << progress << "%";
        cout.flush(); // Flush the output buffer

        // Wait for a short duration
        Sleep(seconds * 1000 / total); // Convert to milliseconds
    }
    cout << endl; // Move to the next line after loading is complete
}


int main() {
    db Repo;
    Service service;

    e_time  exp_time;

    

    cout << "===============================" << endl;
    cout << "  Hello User! Welcome to the App  " << endl;
    cout << "===============================" << endl;

    string username, pass;
    cout << "\nPlease enter your Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> pass;

    User user(username, pass);

    if (Repo.search(user)) {
        cout << "\nWelcome, " << user.get_username() << "!" << endl;

        int choice = 0;
        do {
            cout << "\nPlease select an option:" << endl;
            cout << "1. File Encryption" << endl;
            cout << "2. File Decryption" << endl;
            cout << "3. Exit" << endl;
            cout << "Your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    cout << "\n-- File Encryption Service --" << endl;

                    string reciever, expiration_time, input_file_path;

                    cout << "Enter the Reciever's Username: ";
                    cin >> reciever;

                    if (!Repo.findByUsername(reciever)) {
                        cout << "\nError: Reciever does not exist in the database." << endl;
                        cout << "Do you want to try again or exit? (1 for Retry, 2 for Exit): ";
                        int retryChoice;
                        cin >> retryChoice;
                        if (retryChoice == 2) {
                            break;
                        } else {
                            continue;
                        }
                    }

                    cout << "Expiration Time : ";
                   
                     expiration_time =  exp_time.formatExpirationTime();
                    cout<<"tIME IS  " <<expiration_time<<endl;

                    cin >> input_file_path;


                    cout << "\nFile encryption will now proceed..." << endl;

                    cout << "Hey, tell me where you want the output file saved: ";
                    string output_file;
                    cin >> output_file;

                    // Simulates the encryption process
                    loadingBar(5);

                    service.Encrypt(username, reciever, input_file_path, expiration_time, output_file);


                    cout << "File has been successfully encrypted!" << endl;

                    break;
                }
                case 2: {
                    cout << "\n-- File Decryption Service --" << endl;

                    string input_file_path;
                    cout << "Enter the file location: ";
                    cin >> input_file_path;

                 
                    // Decryption logic here
                    cout << "Hey, tell me where you want the output file saved: ";
                    string output_file;
                    cin >> output_file;
                     loadingBar(5);    
                    service.Decrypt(username,input_file_path,output_file);

                    cout << "File has been successfully decrypted!" << endl;

                    break;
                }
                case 3:
                    cout << "Exiting the application. Thank you for using the app!" << endl;
                    break;
                default:
                    cout << "\nInvalid choice. Please select a valid option." << endl;
            }
        } while (choice != 3);
    } else {
        cout << "\nError: Invalid username or password!" << endl;
    }

    return 0;

}

