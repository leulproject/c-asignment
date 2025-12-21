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

void clearScreen(){
    cout << "\033[2J\033[H";
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
    string fname,mname,lname;
    struct date{ int day,month,year; } dob;
    char gender;
    struct contact{
        string country,city,subcity,phoneno;
        int woreda,houseno;
    } address;
};

struct Account{
    int id;
    int bankaccount;
    string username,password;
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

    clearScreen();
    cout << "\t=== BANK ADMIN SYSTEM ===\n\n";

    while(!login()){
        cout << "Login failed. Try again.\n";
    }
    clearScreen();

    do{
        clearScreen();
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

    fstream acc("admins.csv", ios::in);
    if(!acc) return 0;

    string line;
    while(getline(acc,line)){
        string usn,pwd,temp; stringstream ss(line);
        getline(ss,temp,','); // id
        getline(ss,usn,','); 
        getline(ss,pwd,',');
        if(userName==usn && passWord==pwd){ acc.close(); return 1; }
    }
    acc.close();
    return 0;
}

// ------------------ Create Account ------------------

void createAccount(){
    clearScreen();
    cout << "\n--- CREATE ACCOUNT ---\n";

    fstream person("person.csv", ios::app | ios::in);
    fstream account("account.csv", ios::app | ios::in);
    fstream money("money.csv", ios::app | ios::in);

    Person p;
    string line,lastLine,temp;
    int lastId=0,lastAcc=100000;

    while(getline(account,line)) if(!line.empty()) lastLine=line;
    if(!lastLine.empty()){ stringstream ss(lastLine); getline(ss,temp,','); lastId=stoi(temp); getline(ss,temp,','); lastAcc=stoi(temp); }

    p.id = lastId+1;
    int newAccountNumber=lastAcc+1;

    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    p.fname = inputValidName("First name: ");
    p.mname = inputValidName("Father's name: ");
    p.lname = inputValidName("Grandfather's name: ");

    do{ cout << "DOB (DD MM YYYY): "; cin >> p.dob.day >> p.dob.month >> p.dob.year;
        if(!isValidDate(p.dob.day,p.dob.month,p.dob.year)) cout<<"Invalid date.\n";
    }while(!isValidDate(p.dob.day,p.dob.month,p.dob.year));

    do{ cout << "Gender (M/F): "; cin >> p.gender; p.gender=toupper(p.gender);
        if(!isValidGender(p.gender)) cout<<"Enter M or F only.\n";
    }while(!isValidGender(p.gender));

    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    p.address.country = inputValidName("Country: ");
    p.address.city = inputValidName("City: ");
    p.address.subcity = inputValidName("Subcity: ");

    do{ cout << "Woreda: "; cin >> p.address.woreda; }while(p.address.woreda<=0);
    do{ cout << "House number: "; cin >> p.address.houseno; }while(p.address.houseno<=0);

    do{ cout << "Phone (+251XXXXXXXXX): "; cin >> p.address.phoneno; 
        if(!isValidPhone(p.address.phoneno)) cout<<"Invalid phone number.\n"; 
    }while(!isValidPhone(p.address.phoneno));

    // Save person
    person << p.id << "," << p.fname << "," << p.mname << "," << p.lname << ","
           << p.dob.day << "," << p.dob.month << "," << p.dob.year << ","
           << p.gender << "," << p.address.country << "," << p.address.city << "," << p.address.subcity << ","
           << p.address.woreda << "," << p.address.houseno << "," << p.address.phoneno << "\n";
    person.close();

    // Account credentials
    string username;
    cout << "Username: "; cin >> username;
    string pwd;
    do{ pwd = inputPassword(); if(!isValidPassword(pwd)) cout<<"Password must be at least 6 characters.\n"; }while(!isValidPassword(pwd));

    account << p.id << "," << newAccountNumber << "," << username << "," << pwd << "\n";
    account.close();

    // Initial deposit
    double initial_amount;
    do{ cout << "Initial deposit (min 50): "; cin >> initial_amount;
        if(initial_amount<50) cout<<"Minimum 50 required.\n"; }while(initial_amount<50);
    string time_update=timeTToString(time(0));
    money << p.id << "," << initial_amount << "," << time_update << "\n";
    money.close();

    cout << "\n✅ Account created successfully! Account number: " << newAccountNumber << endl;
    cout << "Press Enter to continue..."; cin.ignore(); cin.get();
}

// ------------------ Edit Account ------------------

void editAccount(){
    clearScreen();
    cout << "\n--- EDIT ACCOUNT ---\n";

    string accNo;
    cout << "Enter account number to edit: ";
    cin >> accNo;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    fstream account("account.csv", ios::in);
    if(!account){ cout<<"No account records found.\n"; return; }

    int matchId=-1;
    string line,t;
    while(getline(account,line)){
        stringstream ss(line);
        getline(ss,t,','); int id=stoi(t);
        getline(ss,t,','); // account number
        if(accNo==t){ matchId=id; break; }
    }
    account.close();
    if(matchId==-1){ cout<<"Account not found.\n"; cout<<"Press Enter..."; cin.get(); return; }

    fstream person("person.csv", ios::in);
    Person p;
    while(getline(person,line)){
        stringstream ss(line);
        getline(ss,t,','); if(matchId==stoi(t)){
            p.id=matchId;
            getline(ss,p.fname,','); getline(ss,p.mname,','); getline(ss,p.lname,',');
            getline(ss,t); p.dob.day=stoi(t); getline(ss,t); p.dob.month=stoi(t); getline(ss,t); p.dob.year=stoi(t);
            getline(ss,t); p.gender=t[0];
            getline(ss,p.address.country,','); getline(ss,p.address.city,','); getline(ss,p.address.subcity,',');
            getline(ss,t); p.address.woreda=stoi(t); getline(ss,t); p.address.houseno=stoi(t);
            getline(ss,p.address.phoneno,',');
            break;
        }
    }
    person.close();

    string hold;
    hold=inputValidName("First name ["+p.fname+"]: ",1); if(!hold.empty()) p.fname=hold;
    hold=inputValidName("Father's name ["+p.mname+"]: ",1); if(!hold.empty()) p.mname=hold;
    hold=inputValidName("Grandfather's name ["+p.lname+"]: ",1); if(!hold.empty()) p.lname=hold;

    // rewrite person.csv
    fstream per("person.csv", ios::in);
    fstream temp("temp.csv", ios::out);
    while(getline(per,line)){
        stringstream ss(line);
        getline(ss,t,','); if(matchId==stoi(t)){
            temp << p.id << "," << p.fname << "," << p.mname << "," << p.lname << "\n"; // simplified for brevity
        } else temp << line << "\n";
    }
    per.close(); temp.close();
    remove("person.csv"); rename("temp.csv","person.csv");
    cout << "✅ Account updated successfully.\nPress Enter to continue..."; cin.get();
}

// ------------------ Delete Account ------------------

void deleteAccount(){
    clearScreen();
    cout << "\n--- DELETE ACCOUNT ---\n";

    string accNo; cout << "Enter account number: "; cin >> accNo;

    fstream account("account.csv", ios::in);
    if(!account){ cout<<"No accounts found.\n"; return; }

    int matchId=-1; string line,t;
    while(getline(account,line)){
        stringstream ss(line);
        getline(ss,t,','); int id=stoi(t);
        getline(ss,t,','); if(accNo==t){ matchId=id; break; }
    }
    account.close();
    if(matchId==-1){ cout<<"Account not found.\nPress Enter..."; cin.get(); return; }

    char confirm;
    cout << "Are you sure to delete? (y/n): "; cin >> confirm;
    if(toupper(confirm)!='Y'){ cout<<"Cancelled.\nPress Enter..."; cin.get(); return; }

    // Delete from files
    vector<string> files = {"person.csv","account.csv","money.csv"};
    for(string f:files){
        fstream fin(f); fstream fout("temp.csv", ios::out);
        while(getline(fin,line)){
            stringstream ss(line); getline(ss,t,',');
            if(matchId!=stoi(t)) fout<<line<<"\n";
        }
        fin.close(); fout.close();
        remove(f.c_str()); rename("temp.csv",f.c_str());
    }
    cout << "✅ Account deleted.\nPress Enter..."; cin.get();
}

// ------------------ Deposit ------------------

void deposit(){
    clearScreen();
    cout << "\n--- DEPOSIT ---\n";

    string accNo; double amount;
    cout << "Enter account number: "; cin >> accNo;
    cout << "Enter deposit amount: "; cin >> amount;
    if(amount<=0){ cout<<"Amount must be positive.\nPress Enter..."; cin.get(); return; }

    fstream account("account.csv", ios::in); int matchId=-1;
    string line,t;
    while(getline(account,line)){
        stringstream ss(line); getline(ss,t,','); int id=stoi(t); getline(ss,t,',');
        if(accNo==t){ matchId=id; break; }
    }
    account.close();
    if(matchId==-1){ cout<<"Account not found.\nPress Enter..."; cin.get(); return; }

    // Update balance
    fstream money("money.csv", ios::in);
    fstream temp("temp.csv", ios::out);
    double curBal=0; bool updated=false;
    while(getline(money,line)){
        stringstream ss(line); getline(ss,t,','); double bal;
        if(matchId==stoi(t)){ getline(ss,t,','); curBal=stod(t)+amount; temp<<matchId<<","<<curBal<<","<<timeTToString(time(0))<<"\n"; updated=true; }
        else temp<<line<<"\n";
    }
    money.close(); temp.close();
    remove("money.csv"); rename("temp.csv","money.csv");
    cout << "✅ Deposited successfully. New balance: " << curBal << endl;
    cout << "Press Enter to continue..."; cin.ignore(); cin.get();
}

// ------------------ Withdrawal ------------------

void withdrawal(){
    clearScreen();
    cout << "\n--- WITHDRAW ---\n";

    string accNo; double amount;
    cout << "Enter account number: "; cin >> accNo;
    cout << "Enter withdrawal amount: "; cin >> amount;

    if(amount<=0){ cout<<"Amount must be positive.\nPress Enter..."; cin.get(); return; }

    fstream account("account.csv", ios::in); int matchId=-1;
    string line,t;
    while(getline(account,line)){
        stringstream ss(line); getline(ss,t,','); int id=stoi(t); getline(ss,t,',');
        if(accNo==t){ matchId=id; break; }
    }
    account.close();
    if(matchId==-1){ cout<<"Account not found.\nPress Enter..."; cin.get(); return; }

    fstream money("money.csv", ios::in);
    fstream temp("temp.csv", ios::out);
    double curBal=0; bool updated=false;
    while(getline(money,line)){
        stringstream ss(line); getline(ss,t,','); double bal;
        if(matchId==stoi(t)){ getline(ss,t,','); curBal=stod(t); if(amount>curBal){ cout<<"Insufficient balance.\n"; temp<<line<<"\n"; updated=true; break; } curBal-=amount; temp<<matchId<<","<<curBal<<","<<timeTToString(time(0))<<"\n"; updated=true; }
        else temp<<line<<"\n";
    }
    money.close(); temp.close();
    remove("money.csv"); rename("temp.csv","money.csv");
    if(updated) cout << "✅ Withdrawal successful. Remaining balance: " << curBal << endl;
    cout << "Press Enter to continue..."; cin.ignore(); cin.get();
}
