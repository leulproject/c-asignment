#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <iomanip>
using namespace std;
// a template for error check
template <typename T>
T errorcheck(string error_message){//accept the error message
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
T accept(string prompt, string error_message){
    cout << prompt;
    return errorcheck<T>(error_message);
}
void removeTrailingCR(string &s)
{
    if(!s.empty() && s.back() == '\r') // check last character
    {
        s.pop_back(); // remove it only if it is '\r'
    }
}

struct Transaction{
    int accountId;
    int relatedAccountId; // -1 if none
    string type;
    double amount;
    string timestamp;
};

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
int telecom(int index);
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
// a function that change a string to time_t
time_t stringToTimeT(const string& timeStr) {
    tm tmTime = {}; 
    stringstream ss(timeStr);

    ss >> get_time(&tmTime, "%Y-%m-%d %H:%M:%S");

    if (ss.fail()) {
        return -1;
    }

    return mktime(&tmTime);
}
//function that change time_t to string
string timeTToString(time_t t) {
    tm *ltm = localtime(&t);

    stringstream ss;
    ss << put_time(ltm, "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

int transaction(
    int accountId,
    int relatedAccountId,
    string type,
    double amount,
    string timestamp
)
    {
        fstream f;
        f.open("transaction.csv",ios::app);
        if(!f)
        {
            cout<<"Internal Fault, please try again\n";
            return -1;
        }
        f<<accountId<<','
         <<relatedAccountId<<','
         <<type<<','
         <<amount<<','
         <<timestamp<<'\n';
        return 1;
    }


int telecom(int index)
{
        //promting the user 
        cout<<"Enter Phone number (09********) \n";
        string phone;
        do{
        phone = accept<string>("Enter : ","Enter only number");//accept the phone number
        }
        while (!(phone[0] == '0' && (phone[1] == '9' || phone[1] == '7') && phone.length() == 10));
        //check if it is ten digit and begin with 09 or 07
        if(phone[0] == '0' && (phone[1] == '9' || phone[1] == '7') && phone.length() == 10){
            double amount = 0;
            //accepting the amount of money
            amount = accept<double>("Enter amount : ","Enter only number\n");
            
            while(amount>1000 || amount < 5){
                amount = accept<double>("Enter amount betweem 5 - 1000 : ", "Rnter only number\n");
            }
            fstream f;//file stream f
            f.open("money.csv",ios::in);//open that money.csv in that filestream in reading mode
            if(!f.is_open()){//if not opened return 0 
                return 0;
            }
            vector<moneyfile> list;//open a vector to store all lines
            string line = "";
                while(getline(f,line)){//copy row to line string
                    removeTrailingCR(line);
                    moneyfile m;//open temporary moneyfile variable
                    string tempo;
                    stringstream ss(line);
                    getline(ss,tempo,',');
                    m.id = stoi(tempo);
                    getline(ss,tempo,',');
                    m.money = stod(tempo);
                    getline(ss,tempo,'\n');
                    m.lastUpdate = stringToTimeT(tempo);
                    list.push_back(m);//add to vector 
                }
                f.close();//after we finish all adding the file content to the vector close that filestream
            for(moneyfile &m:list){//iterate in each vector elements
                if(index == m.id){//if if the user id is found
                    if(m.money < amount + 100){//check if the user has enough amount
                        cout<<"balance insufficient";
                        return 0;
                    }
                    fstream w;
                    w.open("money.csv",ios::out);
                    if(!w.is_open()){
                        return 0;
                    }
                    //if the user has enough money
                    m.money = m.money - amount;
                    m.lastUpdate = time(0);//update time
                    cout<<"successfully charged\n";
                    
                    transaction(index,0,"TELECOM",amount,timeTToString(m.lastUpdate));
                    
                    for(moneyfile &m:list){
                        w<<m.id<<',';
                        w<<m.money<<',';
                        w<<timeTToString(m.lastUpdate)<<'\n';
                    }
                    return 1;
                }

            }



        }

    
        return 1;
}
int transfer(int index){
    //create a variable for account number and id of reciever
    int acc_number, id = -1;
    string name, f_name;

    cout<<"Enter account number of reciever\n";
    acc_number = accept<int>("Enter : ","Enter only numbers!\n");

    fstream f;
    f.open("account.csv",ios::in);

    if(!f)
    {
        cout<<"Internal Problem happened\n";
        return 0;
    }
    string line  = "";
    removeTrailingCR(line);
    while(getline(f,line))
    {
        int temporary_id,temporary_acc;
        stringstream ss(line);
        string temp;
        getline(ss,temp,',');//copying the first coloumn element to temp
        temporary_id = stoi(temp);//change that value to int
        getline(ss,temp,',');//copying the second coloumn element
        temporary_acc = stoi(temp);//change it to int
        if(temporary_acc == acc_number){//if the user account found
            id = temporary_id;
            f.close();
            break;
        }
        


    }

    if(!f)
    {
            cout<<"The user account not found! ";
            f.close();// if the account not found
            return 0;
    }

    fstream p;
    string temp;
    p.open("person.csv",ios::in);//open file in writing mode
    if(!p)
    {
        cout<<"Internal problem happened , Please try again ";
        return 0;
    }
    string line2;
    removeTrailingCR(line2);
    while(getline(p,line2))
    {
        int checkid;
        stringstream ss2(line2);
        getline(ss2,temp,',');
        checkid = stoi(temp);
        if(checkid == id)
        {
            getline(ss2,name,',');
            getline(ss2,f_name,',');
        }

    }
    p.close();
    cout << "Do you want to send to " << name << " " << f_name << "\n";
    cout<<"1 for continue\n"
        <<"2 for exit\n";
    int choiceName = accept<int>("Enter : ","Enter only number: ");
    if(choiceName == 2){
        return -1;
    }
    else if(choiceName != 1)
    {
        cout<<"Wrong Input!";
        return 0;
    }
        
    cout<<"Enter amount you want to Transfer \n";
    double amount;
    do{
    amount = accept<double>("Enter  : ","Enter only number : ");
    if(amount < 10){
        cout<<"You must write money that is greater than 10";
    }
    }
    while( amount< 10);
    f.open("money.csv",ios::in);
        vector<moneyfile> list;
        string lineOfMoney;
        removeTrailingCR(lineOfMoney);
        while(getline(f,lineOfMoney)){
            stringstream ss3(lineOfMoney);
            string tempo;
            moneyfile m;
            getline(ss3,tempo,',');
            m.id = stoi(tempo);
            getline(ss3,tempo,',');
            m.money = stod(tempo);
            getline(ss3,tempo,'\n');
            m.lastUpdate = stringToTimeT(tempo);
            list.push_back(m);
        }
        for(moneyfile &m:list)
        {
            if(m.id == index)
            {
                if(m.money < 100 + amount)
                {
                    cout<<"Insufficient balance\n";
                    return 1;
                }
                for(moneyfile &mn:list)
                {
                    
                    if(mn.id == id)
                    {
                    m.money -= amount;
                    m.lastUpdate = time(0);
                    transaction(index,mn.id,"TRANSFER",amount,timeTToString(m.lastUpdate));
                    mn.money += amount;
                    mn.lastUpdate = time(0);
                    transaction(mn.id,index,"RECEIVE",amount,timeTToString(mn.lastUpdate));
                    break;
                    }
                }
                break;
            }
        }
        f.close();
        f.open("money.csv",ios::out);
        if(!f)
        {
            cout<<"Internal problem , Please try again\n ";
            return 0;
        }
        for(moneyfile &n:list){
            f<<n.id<<','
                <<n.money<<','
                <<timeTToString(n.lastUpdate)<<endl;
        }
        f.close();
        cout<<"Successfully Transfered";
        return 1;
    }
int password(int index)
{
    fstream f;
    string pss;
    bool right_password = 0;
    
    int trial = 0;
    do
    {
        f.open("account.csv",ios::in);
        if(!f){
            cout<<"Internal problem happened , please try later\n";
            return 0;
        }
        cout<<"Enter old password";
        if(trial != 0){
            cout<<" :try"<<trial+1<<endl;
        }
        pss = accept<string>("Enter ","Enter your old password: ");
        string line;
        while(getline(f,line))
        {
            removeTrailingCR(line);
            stringstream ss(line);
            string temp;
            int id;
            getline(ss,temp,',');
            id = stoi(temp);
            getline(ss,temp,',');
            getline(ss,temp,',');
            getline(ss,temp,'\n');
            if(id == index){
                if(temp == pss)
                {
                    right_password = 1;
                    break;
                }
                else
                {
                    break;
                }
            }
        }
        if(!right_password)
        {
            cout<<"Wrong password \n";
             trial++;
        }
        f.close();
    }
    while(right_password == 0 && trial < 3);
    if(trial > 2)
    {
        cout<<"Exceed your attempt \n";
        return 0;
    }
    string n,c;
    do
    {   do{
            n = accept<string>("New password: ","Enter any letters for password : ");
            if(n.length()<5)
            {
                cout<<"Password must consist greater that 4 characters \n";
            }
        }
        while(n.length()<5);
        c = accept<string>("Confirm password: ","Enter any letters : ");
        if(n!=c)
        {
            cout<<"password doesn't match try Again\n";
        }
    }
    while(n != c);
    f.open("account.csv",ios::in);
    if(!f)
    {
        cout<<"Internal Problem happened \n";
        return 0;
    }
    vector<account> A;
    string line1;
    while(getline(f,line1)){
        removeTrailingCR(line1);
        stringstream sss(line1);
        account m;
        string temp1;
        getline(sss,temp1,',');
        m.id = stoi(temp1);
        getline(sss,temp1,',');
        m.bankaccount = stoi(temp1);
        getline(sss,m.username,',');
        getline(sss,m.password,'\n');
        A.push_back(m);
    }

    f.close();


    f.open("account.csv",ios::out);

    if(!f)
    {
        cout<<"Internal Problem happened \n";
        return 0;
    }
    for(auto &m:A)
    {
        if(m.id == index){
            m.password = n;
        }
        f<<m.id<<','
         <<m.bankaccount<<','
         <<m.username<<','
         <<m.password<<'\n';
    }
    f.close();
    return 1;


    
}
int balance(int index)
{
    fstream f;
    f.open("money.csv",ios::in);
    if(!f){
        cout<<"Internal problem happened \n";
        return 0;
    }
    string line;
    double amount;
    while(getline(f,line))
    {
        removeTrailingCR(line);
        int id;
        double money;
        string timeStamp,temp;
        stringstream ss(line);
        getline(ss,temp,',');
        id = stoi(temp);
        getline(ss,temp,',');
        money = stod(temp);
        getline(ss,timeStamp,'\n');
        if(index == id)
        {
            amount = money;
            break;
        }
        

    }
    f.close();
        cout<<"Your balance is "<<fixed<<setprecision(2)<<amount<<" birr";
        return 1;
}    
int transaction_list(int index)
{
    vector<Transaction>T;
    fstream f;
    f.open("transaction.csv",ios::in);
    if(!f)
    {
        cout<<"Some Internal problem Happened , Please Try Again later\n";
        return 0;
    }
    string line = "";
    while(getline(f,line))
    {
        removeTrailingCR(line);
        stringstream ss(line);
        string temp,type;
        string timeStamp;
        int id,rid;
        double amount;
        getline(ss,temp,',');
        id = stoi(temp);
        if(index == id){
            getline(ss,temp,',');
            rid = stoi(temp);
            getline(ss,type,',');
            getline(ss,temp,',');
            amount = stod(temp);
            getline(ss,timeStamp,'\n');
            Transaction R = {id,rid,type,amount,timeStamp};
            T.insert(T.begin(),R);
        }
        else
        {
        getline(ss,temp,',');
        getline(ss,type,',');
        getline(ss,temp,',');
        getline(ss,timeStamp,'\n');
        }
    }
    cout<<"Transaction List\n"
        <<"Type"<<"\t\t"<<"amount"<<"\t\t"<<"time\n";
    bool is_there_data = 0;
    for(auto t:T){
        if(t.accountId == index)
        {
        cout<<t.type<<"\t"
            <<t.amount<<"\t"
            <<t.timestamp<<"\n";
        is_there_data = 1;
        }
        

    }
    if(!is_there_data){
            cout<<"No transaction \n";
        }

    return 1;
    
}
int myAccount(int index)
{
    cout<<"choose From the option\n";
    cout<<"1 My Balance \n"
        <<"2 Change password \n"
        <<"3 letest Transactions \n";
    int choice = accept<int>("Enter : ","Enter only Numbers \n");
    if(choice == 1)
    {
        int result =  balance(index);
    }
    else if(choice == 2)
    {
        int result = password(index);
    }
    else if(choice == 3)
    {
        int result =  transaction_list(index);
    }
    return 1;
}
void page1(int index){
    cout<<"Choose from the option\n";
    cout<<"1. Telecom service\n";
    cout<<"2. Transfer\n";
    cout<<"3. My account\n";
    int choice;
    choice = accept<int>("Enter : ", "Enter only number value ");
    if(choice == 1){
        telecom(index);
    }
    else if(choice == 2){
        int result =transfer(index);
    }
    else if(choice == 3){
        int result =  myAccount(index);
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

