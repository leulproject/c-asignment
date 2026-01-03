#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <string>
#include <sstream>
using namespace std;

// a template for error check
template <typename T>
T errorcheck(string error_message)
 {//accept the error message
    T val;
    string input;
    bool error_free = false;//stayed false till error found

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
T accept(string prompt, string error_message)
{
    cout << prompt;
    return errorcheck<T>(error_message);
}

struct atm
{
  /* data */
  char header[15];
  int id;
  int pin;
  time_t expired_date;
};

int main() {
    fstream f("atm.bank", ios::app | ios::binary);
    if (!f) {
        cout << "Error opening ATM file!" << endl;
        return 1;
    }
    atm newAtm;
    string name_of_atm;
    name_of_atm = accept<string>("Enter ATM name: ", "Please enter a valid ATM name.");
    strcpy(newAtm.header, "SchoolProject");
    newAtm.id = accept<int>("Enter ATM ID: ", "Enter a valid integer for ATM ID.");

    do
    {
      newAtm.pin = accept<int>("Enter ATM PIN of four digits: ", "Enter a valid integer for ATM PIN.");
    } while (newAtm.pin < 1000 || newAtm.pin > 9999);

    newAtm.expired_date = time(0) + 365 * 24 * 60 * 60; // Set expiration date to one year from now


    f.write(reinterpret_cast<char*>(&newAtm), sizeof(newAtm));
    f.close();
    string filename = name_of_atm+to_string(newAtm.id) + ".atm";
    f.open(filename, ios::out | ios::binary);
    if (!f) {
        cout << "Error creating ATM file!" << endl;
        return 1;
    }
    f.write(reinterpret_cast<char*>(&newAtm.id), sizeof(newAtm.id));
    f.close();

    cout << "ATM created successfully!" << endl;
    return 0;

}