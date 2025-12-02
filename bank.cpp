#include <iostream>
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

    return val;
}

template <typename T>
T accept(string prompt, string error_message){
    cout << prompt;
    return errorcheck<T>(error_message);
}
struct address
{
    string country;
    string city;
    string subcity;
    string wereda;
    int housenumber;
};

struct person
{
    string name;
    struct dateofbirth
    {
        int date;
        int month;
        int year;
    };
    address paddress;
    string gender;  
};
struct transaction
{
    int trasaction_number;
    transaction* after = NULL;
};
struct account
{
    person customer;
    int bankaccount;
    
    account* next = NULL;
};



int main(){

    return 0;
}
