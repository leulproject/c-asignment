#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <cstring>
#ifdef _WIN32
#include <windows.h>
#endif
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

struct moneyfile
{
    int id;
    double money;
    time_t lastUpdate;
};
struct Transaction
{
    int accountId;
    int relatedAccountId;
    char type[20];
    double amount;
    struct {
        time_t raw;
    } timestamp;
};
struct atm
{
  /* data */
  char header[15];
  int id;
  int pin;
  time_t expired_date;
};
int login(string);
int home(int index);

int main(int argc, char* argv[]) {
#ifdef _WIN32
    // enable ANSI escape sequences (VT processing) on Windows consoles
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif

  if(argc < 2) {
        cout << "ATM is not inserted!" << endl;
        return 1;
    }

  string filename = argv[1];
  int index;
  index = login(filename);
  if(index == 0) {
      return 1;
  }
  
  system("cls");
  int result = 0; 
  result = home(index);
  return 0;
}
int withdraw(int index) {
    int choice = 0;
    fstream f;
    int amount = 0;
    do{
     system("cls");
    if(choice != 0) {
        cout << "\033[31mInvalid choice! Please select a valid option.\033[0m" << endl;
    }
    cout << "How much do you want to withdraw?\n";
    cout << "1) 100birr \t\t\t 5) 2000birr\n";
    cout << "2) 200birr \t\t\t 6) 4000birr\n";
    cout << "3) 500birr \t\t\t 7) 5000birr\n";
    cout << "4) 1000birr \t\t 8) 10,000birr\n";
    cout << "9) Exit\n";
    choice = accept<int>("Enter your choice (1-9): ", "Enter a valid integer choice.");
    
    
    switch(choice) {
        case 1:
            amount = 100;
            break;
        case 2:
            amount = 200;
            break;
        case 3:
            amount = 500;
            break;
        case 4:
            amount = 1000;
            break;
        case 5:
            amount = 2000;
            break;
        case 6:
            amount = 4000;
            break;
        case 7:
            amount = 5000;
            break;
        case 8:
            amount = 10000;
            break;
        case 9:
            cout << "Cancelling withdrawal." << endl;
            return 1; // user cancelled
        default:
            cout << "Invalid choice!" << endl;
            break;
    }
  } while(choice < 1 || choice > 9);
  f.open("money.bank", ios::in | ios::out | ios::binary);
  if(!f) {
      cout << "Error accessing money file!" << endl;
      return 1;
  }
  moneyfile m;
  bool found = false;
  streampos pos;
  while(f.read(reinterpret_cast<char*>(&m), sizeof(m))) {
      if(m.id == index) {
          found = true;
          pos = f.tellg() - static_cast<streampos>(sizeof(moneyfile));
          break;
      }
  }
  if(!found) {
      cout << "\033[31mInternal problem happened!\033[0m" << endl;
      f.close();
      return 1;
  }
  if(m.money < amount + 100) { // Ensure minimum balance of 100birr remains
      cout << "\033[31mInsufficient balance!\033[0m" << endl;
      f.close();
      return 1;
  }
  m.money -= amount;
  m.lastUpdate = time(0);
  f.seekp(pos);
  f.write(reinterpret_cast<char*>(&m), sizeof(moneyfile));
  f.close();
  f.open("transaction.bank", ios::app | ios::binary);
  if(!f) {
      cout << "Error accessing transactions file!" << endl;
      return 1;
  }
  Transaction t;
  t.accountId = index;
  t.relatedAccountId = 0; // No related account for withdrawal
  strncpy(t.type, "Withdrawal", sizeof(t.type)-1);
  t.type[sizeof(t.type)-1] = '\0';
  t.amount = amount;
  t.timestamp.raw = time(0);
  f.write(reinterpret_cast<char*>(&t), sizeof(Transaction));
  f.close();

  cout << "\033[32mWithdrawal successful!\033[0m" << endl;
  return 0;
}

int home(int index) {
    while (true) {
        cout << "Welcome to your bank account!" << endl;
        cout << "Choose an option:" << endl;
        cout << "1. Withdraw Money" << endl;
        cout << "2. Check Balance" << endl;
        cout << "3. Latest Transactions" << endl;
        cout << "4. Exit" << endl;
        int choice = accept<int>("Enter your choice: ", "Enter a valid integer choice.");
        switch(choice) {
            case 1: {
                int res = withdraw(index);
                // res == 0 -> success, res == 1 -> cancelled
                break;
            }
            case 2: {
                fstream f("money.bank", ios::in | ios::binary);
                if(!f) {
                    cout << "Error accessing money file!" << endl;
                    break;
                }
                moneyfile m;
                bool found = false;
                while(f.read(reinterpret_cast<char*>(&m), sizeof(m))) {
                    if (m.id == index) {
                        found = true;
                        cout << "Balance: " << m.money << " birr" << endl;
                        if (m.lastUpdate != 0) {
                            cout << "Last update: " << ctime(&m.lastUpdate);
                        }
                        break;
                    }
                }
                f.close();
                if(!found) {
                    cout << "Account balance not found." << endl;
                }
                break;
            }
            case 3: {
                fstream f("transaction.bank", ios::in | ios::binary);
                if(!f) {
                    cout << "Error accessing transactions file!" << endl;
                    break;
                }
                vector<Transaction> recs;
                Transaction t;
                while(f.read(reinterpret_cast<char*>(&t), sizeof(t))) {
                    if (t.accountId == index || t.relatedAccountId == index) {
                        recs.push_back(t);
                    }
                }
                f.close();
                if (recs.empty()) {
                    cout << "No transactions found." << endl;
                    break;
                }
                sort(recs.begin(), recs.end(), [](const Transaction& a, const Transaction& b){
                    return a.timestamp.raw > b.timestamp.raw;
                });
                cout << "Latest transactions:" << endl;
                int limit = min((int)recs.size(), 5);
                for (int i=0;i<limit;i++) {
                    cout << i+1 << ". " << recs[i].type << " ";
                    cout << recs[i].amount << " birr ";
                    cout << "on " << ctime(&recs[i].timestamp.raw);
                }
                break;
            }
            case 4:
                cout << "Exiting. Thank you!" << endl;
                return 0;
            default:
                cout << "Invalid choice!" << endl;
                break;
        }
        cout << "Press any key to continue..." << endl;
        _getch();
        system("cls");
    }
}

int login(string filename) {
  fstream f;
  f.open(filename, ios::in | ios::binary);
  if (!f) {
      cout << "Error opening ATM!" << endl;
      return 0;
  }
  int id;
  f.read(reinterpret_cast<char*>(&id), sizeof(id));
  if(id < 0 || id > 1000000){
      cout << "Invalid ATM!" << endl;
      cout<< "Go to the nearest bank" << endl;
      f.close();
      return 0;
  }
  f.close();
  f.open("atm.bank", ios::in | ios::binary);
  if (!f) {
      cout << "Your quest is not complete!" << endl;
      cout<< "visit out nearest bank" << endl;
      return 0;}
  atm a;
  bool found = false;
  while(f.read(reinterpret_cast<char*>(&a), sizeof(a))) {
      if (a.id == id) {
          found = true;
          break;
      }
  }
  f.close();
  if(!found) {
      cout << "Your quest is not complete!" << endl;
      cout<< "visit out nearest bank" << endl;
      return 0;
  }
  time_t now = time(0);
  if(now > a.expired_date) {
      cout << "ATM has expired!" << endl;
      cout<< "visit out nearest bank" << endl;
      return 0;
  } 
  cout << "Welcome to the bank ATM!" << endl;
  cout << "Enter your PIN: ";
  int pin = accept<int>("", "Enter a valid integer for PIN.");
  if(pin != a.pin) {
    int trial = 1;
    while(trial < 3) {
        cout << "Wrong PIN! Try again: ";
        pin = accept<int>("", "Enter a valid integer for PIN.");
        if(pin == a.pin) {
            break;
        }
        trial++;
    }
    if(trial == 3) {

      cout << "You have exceeded the maximum number of attempts!" << endl;
      return 0;
    }
  }
  return id;
}