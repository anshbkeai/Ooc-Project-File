#include "Token.h"
#include "Timer.h"
#include<iostream>
#include<string>
//#include "aes.h"
class  Test {
    Timer  time;
/*
    public:
    Aes_Token  aes_token;
    string  genrate_Token(Token  token) {

    

        string  username_reciver  =  token.sender+"."+token.reciver;
        string  time  =  token.issue_time+"."+token.expiration_time;
        string  final_token  =   username_reciver+"..."+time;

        //generating key and iv

            std::string key = aes_token.generateRandomBytes(16); // 16 bytes for AES-128
            std::string iv = aes_token.generateRandomBytes(16);  // 16 bytes IV
            // cout<<"KEY  IS    == > " <<key<<"\n  the   IV -S --> "<<iv<<endl;
            // string  s  =  aes_token.encrypt("vedansh lauda",key,iv);
            // cout<<s<<endl;
            // cout<<"thos is for  de"<<endl;
            // cout<<aes_token.decrypt(s,key,iv)<<endl;        
            std::string  e_token  =  aes_token.encrypt(final_token,key,iv);
            
        cout << e_token << " " << key << " " << iv << endl;
            return  "kothrud"+e_token+"mit"+key+"wpu"+iv;
    }
    string decrypt_token(string input){
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

        cout << "E_token is :" << e_token << endl;
        cout << "Key is :" << key << endl;
        cout << "IV is :" << iv<< endl;

    std::string key = generateRandomBytes(16); // 16 bytes for AES-128
        std::string iv = generateRandomBytes(16);  // 16 bytes IV

    /*
    string  e_username  =  encpty(username_reciver, key , iv );
    string  e_time  =  encyrpt(time, key , iv);
    
    string  final_token  =  e_username+"."+e_token+"."+file_format;
    return  encyrpt(final_token, key , iv);
    */
    
/*

        string t = aes_token.decrypt(e_token, key , iv);
        
        cout << t << endl;

        return " ";//
    }


*/
    bool  validate_Token(std::string  token ,  std::string user1) {
        std::string  receiver;
        //  Token  revice   Called  me  up 
        //sender  pass  teh   name
    
    /*
      size_t first_dot = token.find('.');
    size_t second_dot = token.find('.', first_dot + 1);
    size_t last_dot = token.rfind('.');

    // Check for valid format
    if (first_dot == std::string::npos || second_dot == std::string::npos || last_dot == std::string::npos || first_dot >= last_dot) {
        throw std::runtime_error("Invalid token format");
    }

    // Extract user and receiver
    string  user = token.substr(0, first_dot);
    string  receiver = token.substr(first_dot + 1, second_dot - first_dot - 1);

    // Extract time1 and time2
    string  time1 = token.substr(second_dot + 1, last_dot - second_dot - 1);
    string exp_time2 = token.substr(last_dot + 1);
    if(recevier)


    */
        if(receiver.compare(user1) == 0) {
            if(time.compareDates("")) {
                cout<<"ALL  things  Are  fine ."<<endl;
                cout<<"Ready for  the   Decryption"<<endl;
            }
            else {
                return  false;
            }
        }
        else {
            return  false;
        }
        // If all characters match, return true
        return true;
    }
};

