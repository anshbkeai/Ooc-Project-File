#include<iostream>
#include<string>
#include "Token.h"
#include<chrono>
#include<ctime>
#include "Timer.h"
using namespace  std;
int   main() {
    cout<<  "Wnat   to genartw  he   token  "<<endl;
    string  s,r;
    cin>>s;
    cout<<"\n Enter  recider"<<endl;
    cin>>r;
    Token  token(s,r,"","","");
    string   token_gen = token.genrate_Token(); //  call  fo
    if(token.validate_Token(token_gen,"")) {
        cout<<"Verified  token -> "<<token_gen<<endl;
    }
    else {
        cout<<"Not   Verified - > " <<token_gen<<endl;
    }

    auto now = std::chrono::system_clock::now();
    
    // Convert it to time_t to output a human-readable time
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    
    std::cout << "Current time: " << std::ctime(&currentTime);

    currentTime =  time(nullptr);
    string  time  =  ctime(&currentTime);
    cout<<time;

    Timer  timer;
    if(timer.compareDates("Oct 15 03:38:42 2024")) cout<<"Time  is  ther  Open  th e  file";
}