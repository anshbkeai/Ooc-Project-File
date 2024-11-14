#ifndef TOKEN_SERVICE_H
#define TOKEN_SERVICE_H

#include "Token.h"
#include "Timer.h"
#include<iostream>
#include<string>
#include "aes.h"

class  Test {

    public:
    Timer  time;
Aes_Token  aes_token;

    string  genrate_Token(Token  token) {

    

    string  username_reciver  =  token.sender+"."+token.reciver;
    string  time  =  token.issue_time+"."+token.expiration_time;
    string  final_token  =   username_reciver+"."+time;

    //generating key and iv

        std::string key = aes_token.generateRandomBytes(16); // 16 bytes for AES-128
        std::string iv = aes_token.generateRandomBytes(16);  // 16 bytes IV
        // cout<<"KEY  IS    == > " <<key<<"\n  the   IV -S --> "<<iv<<endl;
        // string  s  =  aes_token.encrypt("vedansh lauda",key,iv);
        // cout<<s<<endl;
        // cout<<"thos is for  de"<<endl;
        // cout<<aes_token.decrypt(s,key,iv)<<endl;        
        std::string  e_token  =  aes_token.encrypt(final_token,key,iv);
        
    // cout << e_token << " " << key << " " << iv << endl;
        return  "kothrud"+e_token+"mit"+key+"wpu"+iv;
    }

    //
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

    // cout << "E_token is :" << e_token << endl;
    // cout << "Key is :" << key << endl;
    // cout << "IV is :" << iv<< endl;

    string t = aes_token.decrypt(e_token, key , iv);
   // //.info("TOKEN  IS  THIS  "+t);
    //cout << t << endl;

    return t;
    }



    bool  validate_Token(std::string  token ,  std::string user1) {
        //std::string  receiver;
        //  Token  revice   Called  me  up 
        //sender  pass  teh   name
    
    
      size_t first_dot = token.find('.');
    size_t second_dot = token.find('.', first_dot + 1);
    size_t last_dot = token.rfind('.');

   
    // Extract user and receiver
    string  user = token.substr(0, first_dot);
    string  receiver = token.substr(first_dot + 1, second_dot - first_dot - 1);

    // Extract time1 and time2
    string  time1 = token.substr(second_dot + 1, last_dot - second_dot - 1);
    string exp_time2 = token.substr(last_dot + 1);

    //.warn( user+"  "+receiver+" " +  time1+ "  "+exp_time2);

    if(receiver.compare(user1))


    
        if(user.compare(user1) == 0) {
            if(time.compareDates(exp_time2)) {
                //.info("ALL  things  Are  fine .");
                //.info("Ready for  the   Decryption");
            }
            else {
               //.error("Expiration  Time  Exeecd");
                return  false;
            }
        }
        else {
           //.error("Wrong  Sender ");
            return  false;
        }
        // If all characters match, return true
        //.info("Token  Vaildate  Succesfully");
        return true;
    }
};

#endif
// int  main() {
//     Timer   time;
//     cout<<"The   Current   time  is  "<<time.current_Time()<<endl;
//     Token token("ansh","gaurav","now",time.current_Time(),"file");
//     Test T1;
//     string   t =  T1.genrate_Token(token);
//     cout<<"Token  is  "<< t <<endl;

//     string  d_token   =   T1.decrypt_token(t);
//     cout<<"The Main  Token is   "<<d_token<<endl;

//    if( T1.validate_Token(d_token,"ansh")) {
//     cout<<"TokeN Mathched ";
//    }

    





// }