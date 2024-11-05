#ifndef Vector_H 
#define  Vector_H
#include<iostream>
#include<string>
#include<vector>
#include "User.h"
using  namespace std;

class  db  {
    vector<User>  Db;

    public  :
        db() {
            User  u1("admin","a@123");
            User u2("ansh","a@123");
            User  u3("gaurav","g@123");
            Db.push_back(u1);
            Db.push_back(u2);
            Db.push_back(u3);

        }
        vector<User>  get_user() {
            return  Db;
        }
        void  add(User  u1) {
            Db.push_back(u1);
        }
        bool  isEqual(User  u1,  User  u2) {
            int   val  =   ((u1.get_username().compare(u2.get_username())));
            int  val1  =  ((u1.get_password().compare(u2.get_password())));
            if(val == 0 &&  val1 == 0 ) return  true;
            return  false;
        }
    bool search(User  user) {
       // bool  found  =  false;
        for(int i  = 0 ;i < Db.size() ;i++) {
            if(isEqual(Db.at(i),user)) return  true;

        }
        return  false;
       
    }
    void  delete_user(User  u) {
        if(search(u)) {
            for(int i  = 0 ; i < Db.size();i++) {
                 if(isEqual(Db.at(i),u)){
                    auto  pos  =  Db.begin()+i;
                    Db.erase(pos);
                    cout<<"Dleted  "<<endl;
                    return;
                 }
            }
        }
        else {
            cout<<"Not  in  DB"<<endl;
        }
        return ;
    }

    bool  findByUsername(string  username) {
        for(int i  = 0 ;i <  Db.size();i++) {
            if(Db.at(i).get_username().compare(username) == 0 ) return  true;
        }
        return  false;

    }
};


#endif
