#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <limits>
#include <cctype>
#include <conio.h>
#include <algorithm>
#include <cstring>
using namespace std;

// ------------------ Utility Functions ------------------

template <typename T>
T errorcheck(const string& error_message){
    T val;
    string input;
    bool error_free = false;
    do{
        getline(cin,input);
        stringstream ss(input);
        if(ss >> val && ss.eof()) error_free = true;
        else cout << error_message << endl;
    } while(!error_free);
    return val;
}

template <typename T>
T accept(const string& prompt, const string& error_message){
    cout << prompt;
    return errorcheck<T>(error_message);
}

string capitalize(string s){
    if(!s.empty()) s[0] = toupper(s[0]);
    for(size_t i=1;i<s.size();i++) s[i]=tolower(s[i]);
    return s;
}

string inputValidName(const string& prompt,int allowEmpty=0){
    string s;
    do{
        cout << prompt;
        getline(cin,s);
        if(allowEmpty && s.empty()) return s;
        if(!s.empty() && all_of(s.begin(),s.end(),[](char c){ return isalpha(c); })) return capitalize(s);
        cout << "Letters only. Try again.\n";
    }while(true);
}

string inputPassword(){
    string password;
    char ch;
    cout << "Enter password: ";
    while((ch=_getwch())!='\r'){
        if(ch=='\b'){
            if(!password.empty()){ password.pop_back(); cout<<"\b \b"; }
        } else { password+=ch; cout<<'*'; }
    }
    cout << endl;
    return password;
}

bool isValidGender(char g){
    g = toupper(g);
    return g=='M'||g=='F';
}

bool isLeapYear(int y){
    return (y%4==0 && y%100!=0) || (y%400==0);
}

bool isValidDate(int d,int m,int y){
    if(y<1900 || y>2100) return false;
    if(m<1 || m>12) return false;
    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if(m==2 && isLeapYear(y)) daysInMonth[1]=29;
    return d>=1 && d<=daysInMonth[m-1];
}

bool isValidPhone(const string& phone){
    if(phone.length()!=13) return false;
    if(phone.substr(0,4)!="+251") return false;
    for(int i=4;i<13;i++) if(!isdigit(phone[i])) return false;
    return true;
}

bool isValidPassword(const string& p){ return p.length()>=6; }

string timeTToString(time_t t){
    tm *ltm = localtime(&t);
    stringstream ss;
    ss << put_time(ltm,"%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// ------------------ Structures ------------------

struct Person{
    int id;
    char fname[50], mname[50], lname[50];
    struct date{ int day,month,year; } dob;
    char gender;
    struct contact{
        char country[50], city[50], subcity[50], phoneno[20];
        int woreda, houseno;
    } address;
};

struct Account{
    int id;
    int bankaccount;
    char username[50], password[50];
};
struct Admin{
    int id;
    char username[50], password[50];
};
struct Money{
    int id;
    double money;
    time_t lastUpdate;
};

// ------------------ Function Declarations ------------------

int login();
void createAccount();
void editAccount();
void deleteAccount();
void deposit();
void withdrawal();

// ------------------ Main ------------------

int main(){
    int choice;
    string dummy;

    cout << "\t=== BANK ADMIN SYSTEM ===\n\n";

    while(!login()){
        cout << "Login failed. Try again.\n";
    }

    do{
        cout << "\n--- MAIN MENU ---\n";
        cout << "1. Create Account\n";
        cout << "2. Edit Account\n";
        cout << "3. Delete Account\n";
        cout << "4. Deposit\n";
        cout << "5. Withdraw\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "Invalid input. Enter a number between 0-5.\n";
            cout << "Press Enter to continue..."; 
            getline(cin,dummy); cin.get();
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        switch(choice){
            case 1: createAccount(); break;
            case 2: editAccount(); break;
            case 3: deleteAccount(); break;
            case 4: deposit(); break;
            case 5: withdrawal(); break;
            case 0: cout << "Exiting...\n"; break;
            default:
                cout << "Enter only a number between 0-5!\n";
                cout << "Press Enter to continue..."; cin.get();
        }

    }while(choice!=0);

    return 0;
}

// ------------------ Login ------------------

int login(){
    string userName = accept<string>("User Name: ","Enter your Username: ");
    string passWord = accept<string>("Password: ","Enter password: ");

    fstream acc("admins.bank", ios::binary | ios::in);
    if(!acc) return 0;

    Admin a;
    while(acc.read((char*)&a, sizeof(Admin))){
        if(userName == string(a.username) && passWord == string(a.password)){ acc.close(); return 1; }
    }
    acc.close();
    return 0;
}

// ------------------ Create Account ------------------

void createAccount(){
    cout << "\n--- CREATE ACCOUNT ---\n";

    // open person and money for appending; read account file separately to determine last ids
    fstream person("person.bank", ios::binary | ios::app);
    fstream money("money.bank", ios::binary | ios::app);

    Person p = {0};
    int lastId=0,lastAcc=100000;

    // read all records from account.bank to get max id/account number
    fstream accountReader("account.bank", ios::binary | ios::in);
    Account accRec;
    while(accountReader.read((char*)&accRec, sizeof(Account))){
        if(accRec.id > lastId) lastId = accRec.id;
        if(accRec.bankaccount > lastAcc) lastAcc = accRec.bankaccount;
    }
    accountReader.close();

    p.id = lastId+1;
    int newAccountNumber=lastAcc+1;


    string temp;
    cout << "First name: "; getline(cin, temp); strncpy(p.fname, temp.c_str(), 49); p.fname[49]='\0';
    cout << "Father's name: "; getline(cin, temp); strncpy(p.mname, temp.c_str(), 49); p.mname[49]='\0';
    cout << "Grandfather's name: "; getline(cin, temp); strncpy(p.lname, temp.c_str(), 49); p.lname[49]='\0';

    do{
        cout << "DOB (DD MM YYYY): "; cin >> p.dob.day >> p.dob.month >> p.dob.year;
        if(!isValidDate(p.dob.day,p.dob.month,p.dob.year)) 
            cout<<"Invalid date.\n";
    }while(!isValidDate(p.dob.day,p.dob.month,p.dob.year));

    do{
        cout << "Gender (M/F): "; cin >> p.gender; p.gender=toupper(p.gender);
        if(!isValidGender(p.gender))
            cout<<"Enter M or F only.\n";
    }while(!isValidGender(p.gender));

    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    cout << "Country: "; getline(cin, temp); strncpy(p.address.country, temp.c_str(), 49); p.address.country[49]='\0';
    cout << "City: "; getline(cin, temp); strncpy(p.address.city, temp.c_str(), 49); p.address.city[49]='\0';
    cout << "Subcity: "; getline(cin, temp); strncpy(p.address.subcity, temp.c_str(), 49); p.address.subcity[49]='\0';

    do{ 
        cout << "Woreda: "; cin >> p.address.woreda; 
    }while(p.address.woreda<=0);
    do{ 
        cout << "House number: "; cin >> p.address.houseno; 
    }while(p.address.houseno<=0);

    do{ 
        cout << "Phone (+251XXXXXXXXX): "; 
        cin >> temp; 
        if(!isValidPhone(temp)) 
            cout<<"Invalid phone number.\n"; 
    }while(!isValidPhone(temp));
    strncpy(p.address.phoneno, temp.c_str(), 19); 
    p.address.phoneno[19]='\0';

    // Save person
    person.write((char*)&p, sizeof(Person));
    person.close();

    // Account credentials
    string username;
    cout << "Username: "; cin >> username;
    string pwd;
    do{ 
        pwd = inputPassword(); 
        if(!isValidPassword(pwd)) 
            cout<<"Password must be at least 6 characters.\n"; 
    }while(!isValidPassword(pwd));

    Account acc = {p.id, newAccountNumber};
    strncpy(acc.username, username.c_str(), 49); 
    acc.username[49]='\0';
    strncpy(acc.password, pwd.c_str(), 49); 
    acc.password[49]='\0';

    // append new account record
    fstream account("account.bank", ios::binary | ios::app);
    account.write((char*)&acc, sizeof(Account));
    account.close();

    // Initial deposit
    double initial_amount;
    do{ 
        cout << "Initial deposit (min 50): "; 
        cin >> initial_amount;
        if(initial_amount<50) 
            cout<<"Minimum 50 required.\n"; 
    }while(initial_amount<50);

    Money mon = {p.id, initial_amount, time(0)};
    money.write((char*)&mon, sizeof(Money));
    money.close();

    cout << "\n✅ Account created successfully! Account number: " << newAccountNumber << endl;
    cout << "Press Enter to continue..."; cin.ignore(); cin.get();
}

// ------------------ Edit Account ------------------

void editAccount(){
    cout << "\n--- EDIT ACCOUNT ---\n";

    string accNo;
    int matchId = -1;

    while(matchId == -1){
        cout << "Enter account number to edit: ";
        cin >> accNo;
        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        fstream account("account.bank", ios::binary | ios::in);
        if(!account){ cout<<"No account records found.\n"; return; }

        Account accRec;
        while(account.read((char*)&accRec, sizeof(Account))){
            if(to_string(accRec.bankaccount) == accNo){ 
                matchId=accRec.id; 
                break; 
            }
        }
        account.close();

        if(matchId == -1) cout<<"Account not found. Try again.\n";
    }

    // Read person using sequential access
    fstream person("person.bank", ios::binary | ios::in | ios::out);
    Person p;
    streampos personPos = -1;
    bool personFound = false;
    while(person.read((char*)&p, sizeof(Person))){
        if(p.id == matchId){
            personFound = true;
            personPos = person.tellg() - static_cast<std::streamoff>(sizeof(Person));
            break;
        }
    }
    if(!personFound){
        cout << "Person record not found.\n";
        person.close();
        cout << "Press Enter..."; cin.get();
        return;
    }

    cout << "\n--- EDIT ACCOUNT DETAILS ---\n";

    string hold, input;

    cout << "First name [" << p.fname << "]: "; getline(cin, hold); if(!hold.empty()) strncpy(p.fname, hold.c_str(), 49);
    cout << "Father's name [" << p.mname << "]: "; getline(cin, hold); if(!hold.empty()) strncpy(p.mname, hold.c_str(), 49);
    cout << "Grandfather's name [" << p.lname << "]: "; getline(cin, hold); if(!hold.empty()) strncpy(p.lname, hold.c_str(), 49);

    // DOB
    do {
        cout << "DOB (DD MM YYYY) [" << p.dob.day << " " << p.dob.month << " " << p.dob.year << "]: ";
        getline(cin, input);
        if (input.empty()) break;
        stringstream ss(input);
        int d, m, y;
        if (ss >> d >> m >> y && isValidDate(d, m, y)) {
            p.dob.day = d; p.dob.month = m; p.dob.year = y;
            break;
        }
        cout << "❌ Invalid date.\n";
    } while (true);

    // Gender
    do {
        cout << "Gender (M/F) [" << p.gender << "]: ";
        getline(cin, input);
        if (input.empty()) break;
        char g = toupper(input[0]);
        if (isValidGender(g)) {
            p.gender = g;
            break;
        }
        cout << "Enter M or F only.\n";
    } while (true);

    // Address
    cout << "Country [" << p.address.country << "]: "; getline(cin, hold); if(!hold.empty()) strncpy(p.address.country, hold.c_str(), 49);
    cout << "City [" << p.address.city << "]: "; getline(cin, hold); if(!hold.empty()) strncpy(p.address.city, hold.c_str(), 49);
    cout << "Subcity [" << p.address.subcity << "]: "; getline(cin, hold); if(!hold.empty()) strncpy(p.address.subcity, hold.c_str(), 49);

    // Woreda
    do {
        cout << "Woreda [" << p.address.woreda << "]: ";
        getline(cin, input);
        if (input.empty()) break;
        try {
            int w = stoi(input);
            if (w > 0) { p.address.woreda = w; break; }
        } catch (...) {}
        cout << "Woreda must be positive.\n";
    } while (true);

    // House number
    do {
        cout << "House number [" << p.address.houseno << "]: ";
        getline(cin, input);
        if (input.empty()) break;
        try {
            int h = stoi(input);
            if (h > 0) { p.address.houseno = h; break; }
        } catch (...) {}
        cout << "House number must be positive.\n";
    } while (true);

    // Phone
    do {
        cout << "Phone [" << p.address.phoneno << "]: ";
        getline(cin, input);
        if (input.empty()) break;
        if (isValidPhone(input)) {
            strncpy(p.address.phoneno, input.c_str(), 19);
            break;
        }
        cout << "Invalid phone number.\n";
    } while (true);

    // Confirm changes
    cout << "\n--- CONFIRM CHANGES ---\n";
    cout << "First Name: " << p.fname << "\n";
    cout << "Father's Name: " << p.mname << "\n";
    cout << "Grandfather's Name: " << p.lname << "\n";
    cout << "DOB: " << p.dob.day << "/" << p.dob.month << "/" << p.dob.year << "\n";
    cout << "Gender: " << p.gender << "\n";
    cout << "Country: " << p.address.country << "\n";
    cout << "City: " << p.address.city << "\n";
    cout << "Subcity: " << p.address.subcity << "\n";
    cout << "Woreda: " << p.address.woreda << "\n";
    cout << "House Number: " << p.address.houseno << "\n";
    cout << "Phone: " << p.address.phoneno << "\n";

    char confirm;
    cout << "\nConfirm changes? (y/n): ";
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (toupper(confirm) != 'Y') {
        person.close();
        cout << "Changes cancelled.\nPress Enter to continue...";
        cin.get();
        return;
    }

    // Write back the updated person
    person.seekp(personPos);
    person.write((char*)&p, sizeof(Person));
    person.close();

    cout << "✅ Account updated successfully.\nPress Enter to continue..."; cin.get();
}

// ------------------ Delete Account ------------------

void deleteAccount(){
    cout << "\n--- DELETE ACCOUNT ---\n";

    string accNo;
    int matchId = -1;

    while(matchId == -1){
        cout << "Enter account number: ";
        cin >> accNo;

        fstream account("account.bank", ios::binary | ios::in);
        if(!account){ cout<<"No accounts found.\n"; return; }

        Account accRec;
        while(account.read((char*)&accRec, sizeof(Account))){
            if(to_string(accRec.bankaccount) == accNo){ matchId=accRec.id; break; }
        }
        account.close();

        if(matchId == -1) cout<<"Account not found. Try again.\n";
    }

    // Load person details
    fstream person("person.bank", ios::binary | ios::in);
    Person p;
    bool personFound = false;
    while(person.read((char*)&p, sizeof(Person))){
        if(p.id == matchId){
            personFound = true;
            break;
        }
    }
    person.close();
    if(!personFound){
        cout << "Person record not found.\nPress Enter..."; cin.get();
        return;
    }

    // Load money details
    fstream money("money.bank", ios::binary | ios::in);
    Money m;
    bool moneyFound = false;
    while(money.read((char*)&m, sizeof(Money))){
        if(m.id == matchId){
            moneyFound = true;
            break;
        }
    }
    money.close();
    if(!moneyFound){
        cout << "Money record not found.\nPress Enter..."; cin.get();
        return;
    }

    // Display account details
    cout << "\nAccount Details:\n";
    cout << "First Name: " << p.fname << "\n";
    cout << "Last Name: " << p.lname << "\n";
    cout << "Date of Birth: " << p.dob.day << "/" << p.dob.month << "/" << p.dob.year << "\n";
    cout << "Gender: " << p.gender << "\n";
    cout << "Country: " << p.address.country << "\n";
    cout << "City: " << p.address.city << "\n";
    cout << "Subcity: " << p.address.subcity << "\n";
    cout << "Woreda: " << p.address.woreda << "\n";
    cout << "House Number: " << p.address.houseno << "\n";
    cout << "Phone: " << p.address.phoneno << "\n";
    cout << "Balance: $" << fixed << setprecision(2) << m.money << "\n";

    char confirm;
    cout << "\nAre you sure to delete this account? (y/n): "; cin >> confirm;
    if(toupper(confirm)!='Y'){ cout<<"Cancelled.\nPress Enter..."; cin.get(); return; }

    // Delete from person.bank
    fstream delPerson("person.bank", ios::binary | ios::in);
    fstream tempPerson("temp.bank", ios::binary | ios::out);
    Person delP;
    while(delPerson.read((char*)&delP, sizeof(Person))){
        if(delP.id != matchId) tempPerson.write((char*)&delP, sizeof(Person));
    }
    delPerson.close(); tempPerson.close();
    remove("person.bank"); rename("temp.bank", "person.bank");

    // Delete from account.bank
    fstream delAccount("account.bank", ios::binary | ios::in);
    fstream tempAccount("temp.bank", ios::binary | ios::out);
    Account delAcc;
    while(delAccount.read((char*)&delAcc, sizeof(Account))){
        if(delAcc.id != matchId) tempAccount.write((char*)&delAcc, sizeof(Account));
    }
    delAccount.close(); tempAccount.close();
    remove("account.bank"); rename("temp.bank", "account.bank");

    // Delete from money.bank
    fstream delMoney("money.bank", ios::binary | ios::in);
    fstream tempMoney("temp.bank", ios::binary | ios::out);
    Money delM;
    while(delMoney.read((char*)&delM, sizeof(Money))){
        if(delM.id != matchId) tempMoney.write((char*)&delM, sizeof(Money));
    }
    delMoney.close(); tempMoney.close();
    remove("money.bank"); rename("temp.bank", "money.bank");

    cout << "✅ Account deleted.\nPress Enter..."; cin.get();
}

// ------------------ Deposit ------------------

void deposit(){
    cout << "\n--- DEPOSIT ---\n";

    string accNo;
    int matchId = -1;

    while(matchId == -1){
        cout << "Enter account number: ";
        cin >> accNo;

        fstream account("account.bank", ios::binary | ios::in);
        Account accRec;
        while(account.read((char*)&accRec, sizeof(Account))){
            if(to_string(accRec.bankaccount) == accNo){ matchId=accRec.id; break; }
        }
        account.close();

        if(matchId == -1) cout<<"Account not found. Try again.\n";
    }

    // Load person to display
    fstream person("person.bank", ios::binary | ios::in);
    Person p;
    while(person.read((char*)&p, sizeof(Person))){
        if(p.id == matchId) break;
    }
    person.close();

    cout << "Depositing to account of: " << p.fname << " " << p.mname << " " << p.lname << endl;

    // Load current balance
    fstream moneyCheck("money.bank", ios::binary | ios::in);
    Money mCheck;
    while(moneyCheck.read((char*)&mCheck, sizeof(Money))){
        if(mCheck.id == matchId) break;
    }
    moneyCheck.close();

    cout << "Current balance: " << fixed << setprecision(2) << mCheck.money << endl;

    double amount;
    do{
        cout << "Enter deposit amount: ";
        cin >> amount;
        if(amount <= 0) cout<<"Amount must be positive. Try again.\n";
    }while(amount <= 0);

    // Update balance using sequential access
    fstream money("money.bank", ios::binary | ios::in | ios::out);
    Money m;
    streampos moneyPos = -1;
    bool moneyFound = false;
    while(money.read((char*)&m, sizeof(Money))){
        if(m.id == matchId){
            moneyFound = true;
            moneyPos = money.tellg() - static_cast<std::streamoff>(sizeof(Money));
            break;
        }
    }
    if(!moneyFound){
        cout << "Money record not found.\n";
        money.close();
        cout << "Press Enter to continue..."; cin.ignore(); cin.get();
        return;
    }
    m.money += amount;
    m.lastUpdate = time(0);
    money.seekp(moneyPos);
    money.write((char*)&m, sizeof(Money));
    money.close();

    cout << "✅ Deposited successfully. New balance: " << m.money << endl;
    cout << "Press Enter to continue..."; cin.ignore(); cin.get();
}

// ------------------ Withdrawal ------------------

void withdrawal(){
    cout << "\n--- WITHDRAW ---\n";

    string accNo;
    int matchId = -1;

    while(matchId == -1){
        cout << "Enter account number: ";
        cin >> accNo;

        fstream account("account.bank", ios::binary | ios::in);
        Account accRec;
        while(account.read((char*)&accRec, sizeof(Account))){
            if(to_string(accRec.bankaccount) == accNo){ matchId=accRec.id; break; }
        }
        account.close();

        if(matchId == -1) cout<<"Account not found. Try again.\n";
    }

    // Load person to display
    fstream person("person.bank", ios::binary | ios::in);
    Person p;
    while(person.read((char*)&p, sizeof(Person))){
        if(p.id == matchId) break;
    }
    person.close();

    cout << "Withdrawing from account of: " << p.fname << " " << p.mname << " " << p.lname << endl;

    // Load current balance
    fstream moneyCheck("money.bank", ios::binary | ios::in);
    Money mCheck;
    while(moneyCheck.read((char*)&mCheck, sizeof(Money))){
        if(mCheck.id == matchId) break;
    }
    moneyCheck.close();

    cout << "Current balance: " << fixed << setprecision(2) << mCheck.money << endl;

    double amount;
    do{
        cout << "Enter withdrawal amount: ";
        cin >> amount;
        if(amount <= 0) cout<<"Amount must be positive. Try again.\n";
        else if(amount > mCheck.money) cout<<"Insufficient balance. Try again.\n";
    }while(amount <= 0 || amount > mCheck.money);

    fstream money("money.bank", ios::binary | ios::in | ios::out);
    Money m;
    streampos moneyPos = -1;
    bool moneyFound = false;
    while(money.read((char*)&m, sizeof(Money))){
        if(m.id == matchId){
            moneyFound = true;
            moneyPos = money.tellg() - static_cast<std::streamoff>(sizeof(Money));
            break;
        }
    }
    if(!moneyFound){
        cout << "Money record not found.\n";
        money.close();
        cout << "Press Enter to continue..."; cin.ignore(); cin.get();
        return;
    }
    m.money -= amount;
    m.lastUpdate = time(0);
    money.seekp(moneyPos);
    money.write((char*)&m, sizeof(Money));
    money.close();

    cout << "✅ Withdrawal successful. Remaining balance: " << m.money << endl;
    cout << "Press Enter to continue..."; cin.ignore(); cin.get();
}
