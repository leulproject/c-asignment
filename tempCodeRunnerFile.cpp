#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

template <typename T>
T errorcheck(string error_message){
    T val;
    string input;
    bool error_free = false;

    do {
        getline(cin, input); // read the whole line
        stringstream ss(input);

        if (ss >> val && ss.eof()) { // conversion succeeds and no extra chars
            error_free = true;
        } else {
            cout << error_message << endl;
        }
    } while (!error_free);
    return input;
    }

template <typename T>
T accept(string prompt, string error_message){
    cout << prompt;
    return errorcheck<T>(error_message);
}


struct account
{
    int id;
    int bankaccount;
    string username;
    string password;
};
int login();

int is_login = 0;

int main(){
    int id;
    do{
         id = login();
    }
    while(id == 0);
    return 0;
}
int login(){
    string userName, passWord;
    cout<<"welcome to our bank"<<endl;
    userName = accept<string>("User Name: ","Enter your Username ");
    passWord = accept<string>("Password: ", "Enter password");
    fstream acc;
    acc.open("account.csv",ios::in);
    if(!acc.is_open()){
        return 1;
    }
    string line = "";
    while(getline(acc,line)){
        int idNumber,accountNumber;
        string usn,pss,temp;
        stringstream ss(line);

        if(getline(ss,temp,','))
            idNumber = stoi(temp);
        if(getline(ss,temp,','))
            accountNumber = stoi(temp);
        getline(ss,usn,',');
        getline(ss,temp);
        pss = temp;
        cout<<pss;
        if(userName == usn){
            
            acc.close();
            if(passWord == pss){
                cout<<pss;
                return idNumber;}
            else{
                return 0;}
                
        }
        
    }
acc.close();
return 0;

}

