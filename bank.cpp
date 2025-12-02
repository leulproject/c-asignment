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

int main(){

    return 0;
}
