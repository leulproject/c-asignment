#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include <conio.h>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include <cstring>
using namespace std;

const int SESSION_TIMEOUT = 2 * 60; // 1 minutes in seconds. Change as needed.
// Program start timestamp (fixed timeout measured from program start)
time_t programStart = time(0);
// Global last-activity timestamp (kept for compatibility but no longer used by timeout)
time_t lastActivityGlobal = time(0);

// Returns true and shows a message if the fixed session time has elapsed since program start
bool checkSessionTimeout() {
    if (difftime(time(0), programStart) > SESSION_TIMEOUT) {
        system("cls");
        cout << "Session timed out .\n";
        return true;
    }
    return false;
}

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
            cout << error_message;
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

struct address
{
    char country[50] = {};
    char city[50] = {};
    char subcity[50] = {};
    int wereda;
    int housenumber;
};

struct person
{
    int id;
    struct names
    {
        char first_name[50] = {};
        char middle_name[50] = {};
        char last_name[50] = {};
    }name;
    
    struct date_of_birth
    {
        int date;
        int month;
        int year;
    }bithday;
    address paddress;
    char gender[10];
    int phonenumber;  
};

struct account
{
    int id;
    int bankaccount;
    char username[50] = {};
    char password[50] = {};
};
struct moneyfile
{
    int id;
    double money;
    time_t lastUpdate;
};
struct time_stamp {
    time_t raw; // or break down into year, month, day, etc.
};

struct Transaction { 
    int accountId;
    int relatedAccountId; 
     char type[20];
     double amount;
     time_stamp timestamp; 
    };


int login();
void page1(int);

int main()
{
  int id,counter=0;
  system("cls");
  cout<<"welcome to our bank"<<endl;
  do{

        id = login();
        if(id == -1)
        {
          system("cls");
          cout<<"Internal Problem Happened!, please try later!\n";
        }
        counter++;
        if(id == 0){
          cout << "\033[31m" << "Username and Password doesn't match " << "\033[0m" << endl;
          cout<<3-counter<<" Attempt left\n";
        }
        
  }
  while(id == 0 && counter < 3);
  if(counter > 2){
      system("cls");
      cout << "\033[31m" << "Your finished your trail" << "\033[0m" << endl;
      return 0;
  }
  page1(id);
  return 0;
}
string red(string s) {
    return "\033[31m" + s + "\033[0m";
}
int balance(int index)
{
    if (checkSessionTimeout()) return 0;

    fstream f("money.bank", ios::binary | ios::in);
    if(!f) {
        cout << "Internal problem happened\n";
        return 0;
    }

    moneyfile m;
    bool found = false;
    double amount = 0.0;

    while(f.read(reinterpret_cast<char*>(&m), sizeof(m))) {
        if(m.id == index) {
            amount = m.money;
            found = true;
            break;
        }
    }

    f.close();

    if(found) {
        system("cls");
        cout << "Your balance is \033[32m" << fixed << setprecision(2) << amount << " birr\033[0m\n";
    } else {        system("cls");        cout << "Account not found.\n";
    }

    // Ask if user wants to continue or exit
    int want_exit;
    do {
        cout << "Do you want to continue?\n"
             << "1) Yes\n"
             << "2) No, exit program\n";
        want_exit = accept<int>("Enter: ", "Enter only number: ");
        if (want_exit == 1) {
            system("cls");
            return -5; // back to menu
        }
        else if (want_exit == 2) {
            system("cls");
            return 0;
        }
        else
            cout << "\033[31mWrong Input! Enter 1 or 2\033[0m\n";
    } while (want_exit != 1 && want_exit != 2);

    return 1;
}

void logTransaction(int accountId, int relatedId, const string& type, double amount)
{
    fstream f("transaction.bank", ios::binary | ios::out | ios::app);
    if (!f) {
        cout << "Error opening transaction file\n";
        return;
    }
    Transaction t;
    t.accountId = accountId;
    t.relatedAccountId = relatedId;
    strncpy(t.type, type.c_str(), sizeof(t.type)-1);
    t.type[sizeof(t.type)-1] = '\0';
    t.amount = amount;
    t.timestamp.raw = time(0);

    f.write(reinterpret_cast<char*>(&t), sizeof(Transaction));
    f.close();
}

int transaction_list(int index)
{
    if (checkSessionTimeout()) return 0;

    vector<Transaction> T;
    fstream f("transaction.bank", ios::binary | ios::in);
    if (!f) {
        cout << "Error opening transaction file\n";
        return 0;
    }

    Transaction tr;
    while (f.read(reinterpret_cast<char*>(&tr), sizeof(Transaction))) {
        if (tr.accountId == index) {
            T.insert(T.begin(), tr);
        }
    }
    f.close();

    system("cls");
    cout << "Transaction List\nType\t\tAmount\t\tTime\n";
    if (T.empty()) {
        cout << "No transaction\n";
    } else {
        for (auto &t : T) {
        char buf[64];
        tm *tm_info = localtime(&t.timestamp.raw);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);

        cout << t.type << "\t" << t.amount << "\t" << buf << "\n";
        }
    }

    // Ask if user wants to continue or exit
    int want_exit;
    do {
        cout << "Do you want to continue?\n"
             << "1) Yes\n"
             << "2) No, exit program\n";
        want_exit = accept<int>("Enter: ", "Enter only number: ");
        if (want_exit == 1) {
            system("cls");
            return -5; // back to menu
        }
        else if (want_exit == 2) {
            system("cls");
            return 0;
        }
        else
            cout << "\033[31mWrong Input! Enter 1 or 2\033[0m\n";
    } while (want_exit != 1 && want_exit != 2);

    return 1;
}

string getPassword()
{
    string password;
    char ch;

    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (!password.empty())
            {
                password.pop_back();
                std::cout << "\b \b";
            }
        }
        else
        {
            password += ch;
            std::cout << '*';
        }
    }

    return password;
}
int telecom(int index)
{
    if (checkSessionTimeout()) return 0;

    system("cls");
    cout << "Enter the phone number (09********):\n";

    string phone;
    do {
        phone = accept<string>("Enter: ", "Enter only number");
        if (!(phone.length() == 10 && phone[0] == '0' && (phone[1] == '9' || phone[1] == '7'))) {
            cout << "\033[31mWrong input! Enter the phone number in the format of (09********)\033[0m\n";
        }
    } while (!(phone.length() == 10 && phone[0] == '0' && (phone[1] == '9' || phone[1] == '7')));

    double amount = accept<double>("Enter amount: ", "Enter only number\n");
    while (amount > 1000 || amount < 5) {
        amount = accept<double>("\033[31mEnter amount between 5 - 1000: \033[0m", "Enter only number\n");
    }

    fstream f("money.bank", ios::binary | ios::in | ios::out);
    if (!f.is_open()) {
        cout << "Internal file error!\n";
        return 0;
    }

    moneyfile m;
    streampos pos;
    bool found = false;

    // Find the user account
    while (f.read(reinterpret_cast<char*>(&m), sizeof(m))) {
        if (m.id == index) {
            found = true;
            pos = f.tellg() - static_cast<streampos>(sizeof(moneyfile));
            break;
        }
    }

    if (!found) {
        cout << "\033[31mAccount not found!\033[0m\n";
        f.close();
        return 0;
    }

    // Check balance
    if (m.money < amount + 100) {
        cout << "\033[31mBalance insufficient\033[0m\n";
        f.close();
        return 0;
    }

    // Update balance and timestamp
    m.money -= amount;
    m.lastUpdate = time(0);

    // Write updated struct back to file
    f.seekp(pos);
    f.write(reinterpret_cast<char*>(&m), sizeof(m));
    f.close();

    cout << "\033[32mSuccessfully charged " << fixed << setprecision(2) << amount << " birr\033[0m\n";
    logTransaction(index,0,"Telecom",amount);

    // Ask if user wants to continue
    int want_exit;
    do {
        cout << "Do you want to continue?\n"
             << "1) Yes\n"
             << "2) No, exit program\n";
        want_exit = accept<int>("Enter: ", "Enter only number: ");
        if (want_exit == 1)
            return -5; // back to menu
        else if (want_exit == 2)
            return 0;
        else
            cout << "\033[31mWrong Input! Enter 1 or 2\033[0m\n";
    } while (want_exit != 1 && want_exit != 2);

    return 1;
}
int transfer(int index)
{
    if (checkSessionTimeout()) return 0;

    int acc_number, id = -1;
    system("cls");
    cout << "Enter account number of receiver\n";
    acc_number = accept<int>("Enter: ", "Enter account number in this format (10***): \n");

    // 1) Find receiver account -> id
    fstream fAcc("account.bank", ios::binary | ios::in);
    if (!fAcc) {
        system("cls");
        cout << "I\033[31m Internal Problem happened \033[0m \n";
        return 0;
    }
    account a;
    bool receiverFound = false;
    while (fAcc.read(reinterpret_cast<char*>(&a), sizeof(a))) {
        if (a.bankaccount == acc_number) {
            id = a.id;
            receiverFound = true;
            break;
        }
    }
    fAcc.close();

    if (!receiverFound) {
        system("cls");
        cout << "\033[31mThe user account not found!\033[0m\n";

        int num2;
        do {
            cout << "Do you want to rewrite the account?\n"
                 << "1) Yes\n"
                 << "2) No\n";
            num2 = accept<int>("Enter: ", "Enter only 1 or 2: ");
            if (num2 == 1) return -6;
            else if (num2 == 2) { system("cls"); break; }
            else {
                system("cls");
                cout << "\033[31mWrong input, enter 1 or 2\033[0m\n";
            }
        } while (num2 != 1 && num2 != 2);

        int num;
        do {
            cout << "Do you want to continue in the program?\n"
                 << "1) Yes\n"
                 << "2) No, I want to exit\n";
            num = accept<int>("Enter: ", "Enter only 1 or 2: ");
            if (num == 1) return -5;
            else if (num == 2) return 0; // avoid exit(0)
            else {
                system("cls");
                cout << "\033[31mWrong input, enter 1 or 2\033[0m\n";
            }
        } while (num != 1 && num != 2);
        return 0;
    }

    // 2) Load receiver person info
    fstream fPer("person.bank", ios::binary | ios::in);
    if (!fPer) {
        cout << "\033[31mInternal problem happened, Please try again\033[0m\n";
        return 0;
    }
    person per;
    bool personFound = false;
    while (fPer.read(reinterpret_cast<char*>(&per), sizeof(per))) {
        if (per.id == id) {
            personFound = true;
            break;
        }
    }
    fPer.close();

    if (!personFound) {
        cout << "\033[31mReceiver details not found!\033[0m\n";
        return 0;
    }

    // 3) Prevent self-transfer
    if (index == id) {
        cout << "You can't transfer to yourself\n";
        int num;
        do {
            cout << "Do you want to continue?\n"
                 << "1) Yes\n"
                 << "2) No, I want to exit\n";
            num = accept<int>("Enter: ", "Enter only 1 or 2: ");
            if (num == 1) return -5;
            else if (num == 2) return 0;
            else {
                cout << "\033[32mWrong input, enter 1 or 2\033[0m\n";
            }
        } while (num != 1 && num != 2);
        return 0;
    }

    cout << "Do you want to send to \033[33m"
         << per.name.first_name << " " << per.name.middle_name
         << "\033[0m\n";
    int num;
    do {
        cout << "1) Yes\n"
             << "2) No, I don't want\n";
        num = accept<int>("Enter: ", "Enter only 1 or 2: ");
        if (num == 1) break;
        else if (num == 2) {
            system("cls");
            int num1;
            do {
                cout << "Do you want to continue?\n"
                     << "1) Yes\n"
                     << "2) No, I want to exit\n";
                num1 = accept<int>("Enter: ", "Enter only 1 or 2: ");
                if (num1 == 1) return -5;
                else if (num1 == 2) return 0;
                else {
                    cout << "\033[32mWrong input, enter 1 or 2\033[0m\n";
                    system("cls");
                }
            } while (num1 != 1 && num1 != 2);
            return 0;
        } else {
            cout << "\033[32mWrong input, enter 1 or 2\033[0m\n";
        }
    } while (num != 1 && num != 2);

    system("cls");
    cout << "Enter amount you want to transfer\n";
    double amount;
    do {
        amount = accept<double>("Enter: ", "Enter only number: ");
        if (amount < 10) {
            system("cls");
            cout << "You must write money that is greater than 10\n";
        }
    } while (amount < 10);

    // Optional fixed fee/reserve
    const double feeOrReserve = 100.0; // adjust as needed

    // 4) Debit sender
    fstream fMoney("money.bank", ios::binary | ios::in | ios::out);
    if (!fMoney) {
        cout << "\033[31mInternal problem opening money file\033[0m\n";
        return 0;
    }

    moneyfile m;
    bool senderFound = false;
    std::streampos senderPos;

    while (fMoney.read(reinterpret_cast<char*>(&m), sizeof(m))) {
        if (m.id == index) {
            senderFound = true;
            senderPos = fMoney.tellg() - static_cast<std::streampos>(sizeof(moneyfile));
            break;
        }
    }
    if (!senderFound) {
        cout << "\033[31mSender account not found!\033[0m\n";
        fMoney.close();
        return 0;
    }
    double originalSenderMoney = m.money;
    if (m.money < amount + feeOrReserve) {
        cout << "\033[31mBalance insufficient\033[0m\n";
        fMoney.close();
        return 0;
    }
    m.money -= amount;
    m.lastUpdate = time(0);
    fMoney.seekp(senderPos);
    fMoney.write(reinterpret_cast<char*>(&m), sizeof(moneyfile));
    fMoney.flush();
    if (!fMoney) {
        // attempt rollback
        m.money = originalSenderMoney;
        fMoney.clear();
        fMoney.seekp(senderPos);
        fMoney.write(reinterpret_cast<char*>(&m), sizeof(moneyfile));
        fMoney.close();
        cout << "\033[31mInternal write error, transaction aborted\033[0m\n";
        return 0;
    }

    // 5) Credit receiver
    moneyfile m2;
    bool receiverMoneyFound = false;
    std::streampos receiverPos;

    // Rewind to start before scanning again
    fMoney.clear();
    fMoney.seekg(0, ios::beg);

    while (fMoney.read(reinterpret_cast<char*>(&m2), sizeof(m2))) {
        if (m2.id == id) {
            receiverMoneyFound = true;
            receiverPos = fMoney.tellg() - static_cast<std::streampos>(sizeof(moneyfile));
            break;
        }
    }
    if (!receiverMoneyFound) {
        cout << "\033[31mReceiver account not found! Rolling back...\033[0m\n";
        // rollback sender debit
        m.money = originalSenderMoney;
        fMoney.clear();
        fMoney.seekp(senderPos);
        fMoney.write(reinterpret_cast<char*>(&m), sizeof(moneyfile));
        fMoney.close();
        return 0;
    }

    m2.money += amount;
    m2.lastUpdate = time(0);
    fMoney.seekp(receiverPos);
    fMoney.write(reinterpret_cast<char*>(&m2), sizeof(moneyfile));
    fMoney.flush();
    if (!fMoney) {
        // rollback sender if receiver write failed
        cout << "\033[31mFailed to credit receiver. Rolling back...\033[0m\n";
        m.money = originalSenderMoney;
        fMoney.clear();
        fMoney.seekp(senderPos);
        fMoney.write(reinterpret_cast<char*>(&m), sizeof(moneyfile));
        fMoney.close();
        return 0;
    }
    fMoney.close();

    system("cls");
    cout << "\033[32mSuccessfully transferred\033[0m\n";
    logTransaction(index,id,"Transfer",amount);

    int num3;
    do {
        cout << "Do you want to continue?\n"
             << "1) Yes\n"
             << "2) No, I want to exit\n";
        num3 = accept<int>("Enter: ", "Enter only 1 or 2: ");
        if (num3 == 1) return -5;
        else if (num3 == 2) return 0;
        else {
            cout << "\033[32mWrong input, enter 1 or 2\033[0m\n";
        }
    } while (num3 != 1 && num3 != 2);

    return 0;
}
int password(int index)
{
    if (checkSessionTimeout()) return 0;

    fstream f;
    string pss;
    bool right_password = false;
    int trial = 0;

    // Step 1: Verify old password
    do {
        f.open("account.bank", ios::binary | ios::in);
        if (!f) {
            system("cls");
            cout << "Internal problem happened, please try later\n";
            return 0;
        }

        system("cls");
        if (trial != 0) {
            cout << "Trial " << trial + 1 << "\n";
        }
        cout << "Enter old password: ";
        pss = getPassword();

        account a;
        while (f.read(reinterpret_cast<char*>(&a), sizeof(a))) {
            if (a.id == index) {
                if (pss == string(a.password)) {
                    right_password = true;
                }
                break;
            }
        }
        f.close();

        if (!right_password) {
            cout << "\033[31mWrong password\033[0m\n";
            trial++;
        }
    } while (!right_password && trial < 3);

    if (trial >= 3) {
        cout << "Exceeded your attempts\n";
        return 0;
    }

    // Step 2: Get new password
    string n, c;
    do {
        do {
            system("cls");
            cout << "New password: ";
            n = getPassword();
            cout << endl;
            if (n.length() < 5) {
                cout << "Password must be at least 5 characters\n";
            }
        } while (n.length() < 5);

        cout << "Confirm password: ";
        c = getPassword();
        cout << endl;
        if (n != c) {
            cout << "\033[31mPasswords don't match, try again\033[0m\n";
        }
    } while (n != c);

    // Step 3: Update the account record
    f.open("account.bank", ios::binary | ios::in | ios::out);
    if (!f) {
        cout << "\033[31mInternal problem happened\033[0m\n";
        return 0;
    }

    account a;
    streampos pos;
    bool found = false;
    while (f.read(reinterpret_cast<char*>(&a), sizeof(a))) {
        if (a.id == index) {
            found = true;
            pos = f.tellg() - static_cast<streampos>(sizeof(account));
            break;
        }
    }

    if (!found) {
        cout << "\033[31mAccount not found\033[0m\n";
        f.close();
        return 0;
    }

    // Update password
    strncpy(a.password, n.c_str(), sizeof(a.password) - 1);
    a.password[sizeof(a.password) - 1] = '\0';

    f.seekp(pos);
    f.write(reinterpret_cast<char*>(&a), sizeof(a));
    f.close();

    cout << "\033[32mPassword change completed\033[0m\n";
    return 1;
}

int username(int index)
{
    if (checkSessionTimeout()) return 0;

    fstream f;
    string oldUser;
    bool right_username = false;
    int trial = 0;

    // Step 1: Verify old username
    do {
        f.open("account.bank", ios::binary | ios::in);
        if (!f) {
            system("cls");
            cout << "Internal problem happened, please try later\n";
            return 0;
        }

        system("cls");
        if (trial != 0) {
            cout << "Trial " << trial + 1 << "\n";
        }
        cout << "Enter old username: ";
        oldUser = getPassword(); // reuse your masked input helper

        account a;
        while (f.read(reinterpret_cast<char*>(&a), sizeof(a))) {
            if (a.id == index) {
                if (oldUser == string(a.username)) {
                    right_username = true;
                }
                break;
            }
        }
        f.close();

        if (!right_username) {
            cout << "\033[31mWrong username\033[0m\n";
            trial++;
        }
    } while (!right_username && trial < 3);

    if (trial >= 3) {
        cout << "Exceeded your attempts\n";
        return 0;
    }

    // Step 2: Get new username
    string n, c;
    do {
        do {
            system("cls");
            n = accept<string>("New username: ", "");
            cout << endl;
            if (n.length() < 5) {
                cout << "Username must be at least 5 characters\n";
            }
        } while (n.length() < 5);

        c = accept<string>("Confirm username: ", "");
        cout << endl;
        if (n != c) {
            cout << "\033[31mUsername doesn't match, try again\033[0m\n";
        }
    } while (n != c);

    // Step 3: Update the account record
    f.open("account.bank", ios::binary | ios::in | ios::out);
    if (!f) {
        cout << "\033[31mInternal problem happened\033[0m\n";
        return 0;
    }

    account a;
    streampos pos;
    bool found = false;
    while (f.read(reinterpret_cast<char*>(&a), sizeof(a))) {
        if (a.id == index) {
            found = true;
            pos = f.tellg() - static_cast<streampos>(sizeof(account));
            break;
        }
    }

    if (!found) {
        cout << "\033[31mAccount not found\033[0m\n";
        f.close();
        return 0;
    }

    // Update username safely
    strncpy(a.username, n.c_str(), sizeof(a.username) - 1);
    a.username[sizeof(a.username) - 1] = '\0';

    f.seekp(pos);
    f.write(reinterpret_cast<char*>(&a), sizeof(a));
    f.close();

    cout << "\033[32mUsername change completed\033[0m\n";
    return -5;
}

int myAccount(int index)
{
    int result = -1000;
    do
    {
        if (checkSessionTimeout()) return 0;
        system("cls");
        cout<<"choose From the option\n";
        cout<<"1 My Balance \n"
            <<"2 change username \n"
            <<"3 Change password \n"
            <<"4 letest Transactions \n"
            <<"5 back to home page\n"
            <<"9 exit from program\n";
        int choice = accept<int>("Enter : ","Enter only Numbers \n");
        lastActivityGlobal = time(0);
        if(choice == 1)
        {
            result =  balance(index);
            lastActivityGlobal = time(0);
        }
        else if(choice == 2)
        {
         result = username(index);   
         lastActivityGlobal = time(0);
        }
        else if(choice == 3)
        {
          result = password(index);
          lastActivityGlobal = time(0);
        }
        else if (choice == 4){
       result =  transaction_list(index);
       lastActivityGlobal = time(0);
        }
        else if(choice == 5){
            return -5;
        }
        else if(choice == 9){
            return 0;
        }
        else{
            cout<<red("Wrong Input\n");
            result = -2;
        }
    }
    while (result != -5 && result != 0);
    return result;
}

void page1(int index)
{
    int result = -1000 ;
    system("cls");
    do
    {
        if(result == -5){
            system("cls");
        }
        if(result == -6){
           // result = transfer(index);
        }
        else
        {
            system("cls");
            cout<<"Choose from the option\n"
                <<"1. Telecom service\n"
                <<"2. Transfer\n"
                <<"3. My account\n"
                <<"4. Exit \n";

            int choice;
            choice = accept<int>("Enter : ", "Enter only number value: ");
            
            if(choice == 1){
                result = telecom(index);
            }
            else if(choice == 2){
                result =transfer(index);
            }
            else if(choice == 3){
                result =  myAccount(index);
            }
            else if(choice == 4){
                        return;
            }
            else{
                system("cls");
                cout<<"\033[31mYou have to choose from 1,2,3 only\033[0m\n ";
                result = -2;
            }
        }
    }
    while(result == -2 || result == -5 || result == -6);

}

int login() {
    string userName = accept<string>("User Name: ", "Enter your Username ");
    cout << "Enter password: ";
    string passWord = getPassword();
    cout << endl;

    fstream acc("account.bank", ios::binary | ios::in);
    if (!acc.is_open()) {
        return -1; // file problem
    }

    account a;
    while (acc.read(reinterpret_cast<char*>(&a), sizeof(a))) {
        a.username[sizeof(a.username) - 1] = '\0';
        a.password[sizeof(a.password) - 1] = '\0';

        if (userName == string(a.username) && passWord == string(a.password)) {
            acc.close();
            return a.id;
        }
    }

    acc.close();
    return 0; // not found / authentication failed
}