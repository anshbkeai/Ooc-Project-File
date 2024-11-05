#ifndef TOKEN_H
#define TOKEN_H

#include<iostream>
#include<string>
using  namespace  std;

class Token {
    public  :
        string sender;
        string reciver;
        string  issue_time;
        string  expiration_time;
        string  file_format;
    public:
    Token();
    Token(string sender, string reciver,string issue_time ,  string expiration_time,string  file_format){
        this->sender   =  sender; //  Sender   Will get  Form  the   Account  Login
        this->reciver  =  reciver; //  this  will  we   will  revice  from  the   Reqest
        this->issue_time  =  issue_time; //  current  System  time
        this->expiration_time  =  expiration_time; // we  Will  revice   from  teh   user  
        this->file_format  =  file_format; //  we  will  get   from  the   File   serive  
    }
    string  genrate_Token();
   
};
#endif