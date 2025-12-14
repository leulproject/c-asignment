#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <iomanip>
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


struct account
{
    int id;
    int bankaccount;
    string username;
    string password;
};
struct moneyfile
{
    int id;
    double money;
    time_t lastUpdate;
};

int login();
void telecom(int index);
void page1(int index);


int main(){
    int id;
    do{
         id = login();
    }
    while(id == 0);
    page1(id);
    return 0;
}
time_t stringToTimeT(const string& timeStr) {
    tm tmTime = {}; 
    stringstream ss(timeStr);

    ss >> get_time(&tmTime, "%Y-%m-%d %H:%M:%S");

    if (ss.fail()) {
        return -1;
    }

    return mktime(&tmTime);
}
string timeTToString(time_t t) {
    tm *ltm = localtime(&t);

    stringstream ss;
    ss << put_time(ltm, "%Y-%m-%d %H:%M:%S");

    return ss.str();
}
void charge(){
    cout<<"Charge completed\n";
}
void telecom(int index){

        cout<<"Enter Phone number\n";
        string phone = accept<string>("Enter : ","Enter only number");
       
        
        
        if(phone[0] == '0' && (phone[1] == '9' || phone[1] == '7') && phone.length() == 10){
            double amount = 0;
            amount = accept<double>("Enter amount : ","Enter only number\n");
            while(amount>1000 || amount < 5){
                amount = accept<double>("Enter amount betweem 5 - 1000 : ", "Rnter only number\n");
            }
            fstream f;
            f.open("money.csv",ios::in);
            if(!f.is_open()){
                return;
            }
            vector<moneyfile> list;
            string line = "";
                while(getline(f,line)){
                    moneyfile m;
                    string tempo;
                    stringstream ss(line);
                    getline(ss,tempo,',');
                    m.id = stoi(tempo);
                    getline(ss,tempo,',');
                    m.money = stod(tempo);
                    getline(ss,tempo,'\n');
                    m.lastUpdate = stringToTimeT(tempo);
                    list.push_back(m);
                }
                f.close();
            for(moneyfile &m:list){
                if(index == m.id){
                    if(m.money < amount + 100){
                        cout<<"balance insufficient";
                        return;
                    }

                    m.money = m.money - amount;
                    m.lastUpdate = time(0);
                    cout<<"successfully charged\n";
                    fstream w;
                    w.open("money.csv",ios::out);
                    if(!w.is_open()){
                        return;
                    }
                    for(moneyfile &m:list){
                        w<<m.id<<',';
                        w<<m.money<<',';
                        w<<timeTToString(m.lastUpdate)<<'\n';
                    }
                    return;
                }

            }



        }

    

}
void page1(int index){
    cout<<"Choose from the option\n";
    cout<<"1. Telecom service\n";
    cout<<"2. Transform\n";
    cout<<"3. Utility\n";
    int choice;
    choice = accept<int>("Enter : ", "Enter only number value ");
    if(choice == 1){
        telecom(index);
    }
    else if(choice == 2){
        //transform();
    }
    else if(choice == 3){
        //utiliy();
    }
    //if(choice == )


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
        getline(ss,temp,'\n');
        pss = temp;
        if(userName == usn){
            
            acc.close();
            if(passWord == pss){
                return idNumber;}
            else{
                return 0;}
                
        }
        
    }
acc.close();
return 0;

}

