#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <conio.h>
#include <cstring>

using namespace std;

/* ===================== STRUCTS ===================== */

struct Account {
    int id;
    int bankaccount;
    char username[50] = {};
    char password[50] = {};
};

struct MoneyFile {
    int id;
    double money;
    time_t lastUpdate;
};

struct Transaction {
    int accountId;
    int relatedAccountId;
    char type[15];
    double amount;
    time_t timestamp;
};

/* ===================== HELPERS ===================== */

template<typename T>
T accept(const string& prompt, const string& error) {
    T val;
    while (true) {
        cout << prompt;
        cin >> val;
        if (!cin.fail()) break;
        cin.clear();
        cin.ignore(1000, '\n');
        cout << error << endl;
    }
    return val;
}

string getPassword() {
    string pass;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b' && !pass.empty()) {
            pass.pop_back();
            cout << "\b \b";
        } else {
            pass += ch;
            cout << '*';
        }
    }
    return pass;
}

/* ===================== TRANSACTION ===================== */

int transaction(int id, int rid, const char* type, double amount) {
    ofstream f("transaction.bank", ios::binary | ios::app);
    if (!f) return 0;

    Transaction t{};
    t.accountId = id;
    t.relatedAccountId = rid;
    strncpy(t.type, type, sizeof(t.type));
    t.amount = amount;
    t.timestamp = time(0);

    f.write(reinterpret_cast<char*>(&t), sizeof(Transaction));
    f.close();
    return 1;
}

/* ===================== BALANCE ===================== */

int balance(int index) {
    ifstream f("money.bank", ios::binary);
    MoneyFile m;

    while (f.read(reinterpret_cast<char*>(&m), sizeof(MoneyFile))) {
        if (m.id == index) {
            cout << "Your balance is "
                 << fixed << setprecision(2)
                 << m.money << " birr\n";
            return 1;
        }
    }
    cout << "Account not found\n";
    return 0;
}

/* ===================== TELECOM ===================== */

int telecom(int index) {
    double amount = accept<double>("Enter amount: ", "Numbers only\n");

    fstream f("money.bank", ios::binary | ios::in | ios::out);
    MoneyFile m;

    while (f.read(reinterpret_cast<char*>(&m), sizeof(MoneyFile))) {
        if (m.id == index) {
            if (m.money < amount + 100) {
                cout << "Insufficient balance\n";
                return 0;
            }
            m.money -= amount;
            m.lastUpdate = time(0);

            f.seekp(-sizeof(MoneyFile), ios::cur);
            f.write(reinterpret_cast<char*>(&m), sizeof(MoneyFile));

            transaction(index, 0, "TELECOM", amount);
            cout << "Successfully charged\n";
            return 1;
        }
    }
    return 0;
}

/* ===================== TRANSFER ===================== */

int transfer(int index) {
    int acc = accept<int>("Enter receiver account number: ", "Numbers only\n");

    ifstream af("account.bank", ios::binary);
    Account a;
    int rid = -1;

    while (af.read(reinterpret_cast<char*>(&a), sizeof(Account))) {
        if (a.bankaccount == acc) {
            rid = a.id;
            break;
        }
    }
    af.close();

    if (rid == -1 || rid == index) {
        cout << "Invalid receiver\n";
        return 0;
    }

    double amount = accept<double>("Enter amount: ", "Numbers only\n");

    fstream mf("money.bank", ios::binary | ios::in | ios::out);
    MoneyFile sender, receiver;
    streampos spos, rpos;
    bool fs = false, fr = false;

    while (mf.read(reinterpret_cast<char*>(&sender), sizeof(MoneyFile))) {
        if (sender.id == index) {
            spos = mf.tellg() - streamoff(sizeof(MoneyFile));
            fs = true;
        }
        if (sender.id == rid) {
            receiver = sender;
            rpos = mf.tellg() - streamoff(sizeof(MoneyFile));
            fr = true;
        }
    }

    if (!fs || !fr || sender.money < amount + 100) {
        cout << "Transfer failed\n";
        return 0;
    }

    sender.money -= amount;
    receiver.money += amount;
    sender.lastUpdate = receiver.lastUpdate = time(0);

    mf.seekp(spos);
    mf.write(reinterpret_cast<char*>(&sender), sizeof(MoneyFile));
    mf.seekp(rpos);
    mf.write(reinterpret_cast<char*>(&receiver), sizeof(MoneyFile));
    mf.close();

    transaction(index, rid, "TRANSFER", amount);
    transaction(rid, index, "RECEIVE", amount);

    cout << "Transfer successful\n";
    return 1;
}

/* ===================== LOGIN ===================== */

int login() {
    string u, p;
    cout << "Username: ";
    cin >> u;
    cout << "Password: ";
    p = getPassword();
    cout << endl;

    ifstream f("account.bank", ios::binary);
    Account a;

    while (f.read(reinterpret_cast<char*>(&a), sizeof(Account))) {
        if (u == a.username && p == a.password) {
            return a.id;
        }
    }
    return 0;
}

/* ===================== MENU ===================== */

void menu(int index) {
    int c;
    do {
        cout << "\n1 Balance\n2 Telecom\n3 Transfer\n4 Exit\n";
        c = accept<int>("Choose: ", "Invalid\n");

        if (c == 1) balance(index);
        else if (c == 2) telecom(index);
        else if (c == 3) transfer(index);
    } while (c != 4);
}

/* ===================== MAIN ===================== */

int main() {
    cout << "Welcome to Bank System\n";
    int id = login();
    if (id <= 0) {
        cout << "Login failed\n";
        return 0;
    }
    menu(id);
    return 0;
}
