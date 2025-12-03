#include <iostream>
#include <sstream>
#include <string>
using namespace std;

template <typename T>
T errorcheck(string error_message,int isitnumber = 0){
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
    if(!isitnumber)
        return val;
    else{

    }
    }

template <typename T>
T accept(string prompt, string error_message){
    cout << prompt;
    return errorcheck<T>(error_message);
}

struct address
{
    std::string country;
    string city;
    string subcity;
    int wereda;
    int housenumber;
};

struct person
{
    struct names
    {
        std::string first_name;
        std::string middle_name;
        std::string last_name;
    }name;
    
    struct date_of_birth
    {
        int date;
        int month;
        int year;
    }bithday;
    address paddress;
    string gender;
    int phonenumber;  
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
    string username;
    string password;
    account* next ;
};
struct administrator
{
    person admin;
    string username;
    string password;
    string id;
    string branch;
};


account* account_initializer();
account* create_account(int , account*);
void list_of_admin(administrator []);

int main(){
    cout<<"welcome to our bank";
    cout<<"1 for user ";
    cout<<"2 for admin ";

    administrator listadmin[5];
    list_of_admin(listadmin);

    account* last = account_initializer();
    last = create_account(last->bankaccount,last);

    return 0;
}
bool login(){
    cout<<"welcome to our bank"<<endl;
    cout<<"1 for user "<<endl;
    cout<<"2 for admin "<<endl;
    int respond = accept<int>("Enter: ","Enter only number");
    while (respond != 1 && respond != 2){
        cout<<"Enter only 1 or 2 \n";
        respond = accept<int>("Enter: ","Enter only number");
    }
    if(respond == 2){
        cout<<"1. create account\n";
        cout<<"2. edit account\n";
    }
}

account* create_account(int lastaccount, account* then){
    account bank_customer;
    bank_customer.customer.name.first_name =  accept<string>("Name: ","Enter only letters");
    bank_customer.customer.name.middle_name =  accept<string>("Middle Name: ","Enter only letters");
    bank_customer.customer.name.last_name =  accept<string>("Last Name: ","Enter only letters");
    bank_customer.customer.bithday.date = accept<int>("Date of Birth: ", "Enter only number");
    bank_customer.customer.bithday.month = accept<int>("Month of Birth: ", "Enter only numbers");
    bank_customer.customer.bithday.year = accept<int>("year of Birth", "Enter only numbers");
    bank_customer.customer.gender = accept<string>("Gender: ","Enter male or female");
    bank_customer.customer.paddress.country = accept<string>("Country: ","Enter a country");
    bank_customer.customer.paddress.city = accept<string>("City: ","Enter a city");
    bank_customer.customer.paddress.subcity = accept<string>("subcity: ","Enter a Subcity");
    bank_customer.customer.paddress.wereda = accept<int>("Wereda: ","Enter a number");
    bank_customer.customer.paddress.housenumber = accept<int>("House number: ","Enter a number");
    bank_customer.customer.phonenumber = accept<int>("Phonenumber : ","Enter  only Numbers");
    bank_customer.bankaccount = lastaccount+1213;
    bank_customer.username = accept<string>("Enter username"," ");
    bank_customer.password = accept<string>("Enter password", " ");
    bank_customer.next = then;
    account* customer = new account;
    *customer = bank_customer;
    return customer;
}
void list_of_admin(administrator admin[]){
admin[1] = { { {"Abebe","Kebede","Alemu"}, {12,5,1985}, {"Ethiopia","Addis Ababa","Bole",3,2211}, "male" }, "abebe_k", "pass123", "ADM001", "Bole" };

admin[2] = { { {"Marta","Sime","Tsegaye"}, {7,11,1990}, {"Ethiopia","Addis Ababa","Lideta",6,1203}, "female" }, "marta_s", "marta@90", "ADM002", "Lideta" };

admin[3] = { { {"Getahun","Bekele","Hunde"}, {20,3,1978}, {"Ethiopia","Addis Ababa","Arada",4,9876}, "male" }, "geta_b", "g!eta78", "ADM003", "Arada" };

admin[4] = { { {"Hana","Mesfin","Tadese"}, {15,8,1988}, {"Ethiopia","Addis Ababa","Gulele",2,4432}, "female" }, "hana_m", "hana1988", "ADM004", "Gulele" };

admin[0] = { { {"Samuel","Wondimu","Hailu"}, {1,1,1992}, {"Ethiopia","Addis Ababa","Nifas Silk",5,7788}, "male" }, "sam_w", "sam@92", "ADM005", "Nifas Silk" };

}
account* account_initializer(){
    account* person1 = new account{{{"Abebe","Kebede","Ayele"},{15,3,1985},{"Ethiopia","Addis Ababa","Arada",3,2345},"male"},10000001,NULL};
    account* person2 = new account{{{"Selam","Tadesse","Bekele"},{22,7,1990},{"Ethiopia","Addis Ababa","Bole",5,3456},"female"},10000002,person1};
    account* person3 = new account{{{"Getachew","Meles","Haile"},{10,1,1982},{"Ethiopia","Addis Ababa","Lideta",2,4567},"male"},10000003,person2};
    account* person4 = new account{{{"Marta","Ayele","Tesfaye"},{28,9,1995},{"Ethiopia","Addis Ababa","Kirkos",4,5678},"female"},10000004,person3};
    account* person5 = new account{{{"Dawit","Fikru","Gebre"},{5,5,1988},{"Ethiopia","Addis Ababa","Yeka",7,6789},"male"},10000005,person4};
    account* person6 = new account{{{"Hirut","Bekele","Teshome"},{12,12,1992},{"Ethiopia","Addis Ababa","Akaki",1,7890},"female"},10000006,person5};
    account* person7 = new account{{{"Fikru","Bitew","Hailu"},{18,8,1980},{"Ethiopia","Addis Ababa","Addis Ketema",6,8901},"male"},10000007,person6};
    account* person8 = new account{{{"Tsion","Worku","Gebremedhin"},{23,2,1993},{"Ethiopia","Addis Ababa","Gullele",3,9012},"female"},10000008,person7};
    account* person9 = new account{{{"Hailemariam","Tadesse","Tesfahun"},{30,6,1987},{"Ethiopia","Addis Ababa","Arada",2,1123},"male"},10000009,person8};
    account* person10 = new account{{{"Selamawit","Admasu","Fikadu"},{7,11,1991},{"Ethiopia","Addis Ababa","Bole",5,2234},"female"},10000010,person9};
    account* person11 = new account{{{"Abiy","Hailu","Tadesse"},{14,4,1983},{"Ethiopia","Addis Ababa","Lideta",4,3345},"male"},10000011,person10};
    account* person12 = new account{{{"Eleni","Girma","Meles"},{19,7,1994},{"Ethiopia","Addis Ababa","Kirkos",6,4456},"female"},10000012,person11};
    account* person13 = new account{{{"Kebede","Tesfaye","Haile"},{2,10,1986},{"Ethiopia","Addis Ababa","Yeka",7,5567},"male"},10000013,person12};
    account* person14 = new account{{{"Mulu","Teshome","Bitew"},{25,1,1992},{"Ethiopia","Addis Ababa","Akaki",1,6678},"female"},10000014,person13};
    account* person15 = new account{{{"Eyasu","Fikru","Girma"},{11,3,1989},{"Ethiopia","Addis Ababa","Addis Ketema",3,7789},"male"},10000015,person14};
    account* person16 = new account{{{"Abiyot","Bekele","Tesfahun"},{16,9,1990},{"Ethiopia","Addis Ababa","Gullele",2,8890},"male"},10000016,person15};
    account* person17 = new account{{{"Tsion","Ayele","Hailemariam"},{21,5,1985},{"Ethiopia","Addis Ababa","Arada",5,9901},"female"},10000017,person16};
    account* person18 = new account{{{"Fikadu","Girma","Bekele"},{3,12,1988},{"Ethiopia","Addis Ababa","Bole",6,1012},"male"},10000018,person17};
    account* person19 = new account{{{"Hana","Tesfaye","Teshome"},{9,8,1991},{"Ethiopia","Addis Ababa","Lideta",7,1123},"female"},10000019,person18};
    account* person20 = new account{{{"Biniam","Meles","Haile"},{27,2,1984},{"Ethiopia","Addis Ababa","Kirkos",1,1234},"male"},10000020,person19};
    account* person21 = new account{{{"Sara","Worku","Fikru"},{6,6,1993},{"Ethiopia","Addis Ababa","Yeka",3,1345},"female"},10000021,person20};
    account* person22 = new account{{{"Teshome","Gebre","Bitew"},{13,1,1987},{"Ethiopia","Addis Ababa","Akaki",4,1456},"male"},10000022,person21};
    account* person23 = new account{{{"Mekdes","Haile","Tesfaye"},{20,10,1992},{"Ethiopia","Addis Ababa","Addis Ketema",5,1567},"female"},10000023,person22};
    account* person24 = new account{{{"Gebre","Fikru","Girma"},{8,4,1986},{"Ethiopia","Addis Ababa","Gullele",6,1678},"male"},10000024,person23};
    account* person25 = new account{{{"Hirut","Bekele","Meles"},{17,9,1990},{"Ethiopia","Addis Ababa","Arada",7,1789},"female"},10000025,person24};
    account* person26 = new account{{{"Dawit","Tadesse","Hailemariam"},{29,12,1982},{"Ethiopia","Addis Ababa","Bole",1,1890},"male"},10000026,person25};
    account* person27 = new account{{{"Selam","Ayele","Fikadu"},{12,7,1995},{"Ethiopia","Addis Ababa","Lideta",2,1901},"female"},10000027,person26};
    account* person28 = new account{{{"Abraham","Girma","Bekele"},{4,11,1989},{"Ethiopia","Addis Ababa","Kirkos",3,2012},"male"},10000028,person27};
    account* person29 = new account{{{"Marta","Tesfaye","Tadesse"},{26,3,1991},{"Ethiopia","Addis Ababa","Yeka",4,2123},"female"},10000029,person28};
    account* person30 = new account{{{"Fikru","Haile","Bitew"},{15,5,1985},{"Ethiopia","Addis Ababa","Akaki",5,2234},"male"},10000030,person29};
    account* person31 = new account{{{"Aster","Bekele","Girma"},{22,8,1990},{"Ethiopia","Addis Ababa","Addis Ketema",6,2345},"female"},10000031,person30};
    account* person32 = new account{{{"Hailemariam","Fikru","Tesfahun"},{9,1,1983},{"Ethiopia","Addis Ababa","Gullele",7,2456},"male"},10000032,person31};
    account* person33 = new account{{{"Selamawit","Meles","Teshome"},{18,6,1992},{"Ethiopia","Addis Ababa","Arada",1,2567},"female"},10000033,person32};
    account* person34 = new account{{{"Kebede","Gebre","Haile"},{3,9,1987},{"Ethiopia","Addis Ababa","Bole",2,2678},"male"},10000034,person33};
    account* person35 = new account{{{"Hana","Tadesse","Fikru"},{14,12,1994},{"Ethiopia","Addis Ababa","Lideta",3,2789},"female"},10000035,person34};
    account* person36 = new account{{{"Dawit","Bekele","Girma"},{7,4,1986},{"Ethiopia","Addis Ababa","Kirkos",4,2890},"male"},10000036,person35};
    account* person37 = new account{{{"Mekdes","Ayele","Tesfaye"},{25,10,1991},{"Ethiopia","Addis Ababa","Yeka",5,2901},"female"},10000037,person36};
    account* person38 = new account{{{"Abiy","Fikru","Hailemariam"},{11,2,1988},{"Ethiopia","Addis Ababa","Akaki",6,3012},"male"},10000038,person37};
    account* person39 = new account{{{"Tsion","Haile","Bekele"},{19,7,1993},{"Ethiopia","Addis Ababa","Addis Ketema",7,3123},"female"},10000039,person38};
    account* person40 = new account{{{"Fikadu","Meles","Bitew"},{6,9,1984},{"Ethiopia","Addis Ababa","Gullele",1,3234},"male"},10000040,person39};
    account* person41 = new account{{{"Selam","Girma","Tadesse"},{21,5,1995},{"Ethiopia","Addis Ababa","Arada",2,3345},"female"},10000041,person40};
    account* person42 = new account{{{"Abraham","Haile","Tesfahun"},{30,1,1986},{"Ethiopia","Addis Ababa","Bole",3,3456},"male"},10000042,person41};
    account* person43 = new account{{{"Marta","Fikru","Teshome"},{13,8,1990},{"Ethiopia","Addis Ababa","Lideta",4,3567},"female"},10000043,person42};
    account* person44 = new account{{{"Fikru","Bekele","Girma"},{2,3,1987},{"Ethiopia","Addis Ababa","Kirkos",5,3678},"male"},10000044,person43};
    account* person45 = new account{{{"Hirut","Tadesse","Meles"},{28,12,1992},{"Ethiopia","Addis Ababa","Yeka",6,3789},"female"},10000045,person44};
    account* person46 = new account{{{"Dawit","Ayele","Hailemariam"},{17,6,1983},{"Ethiopia","Addis Ababa","Akaki",7,3890},"male"},10000046,person45};
    account* person47 = new account{{{"Selamawit","Gebre","Fikadu"},{8,9,1991},{"Ethiopia","Addis Ababa","Addis Ketema",1,3901},"female"},10000047,person46};
    account* person48 = new account{{{"Abiy","Girma","Bitew"},{24,4,1989},{"Ethiopia","Addis Ababa","Gullele",2,4012},"male"},10000048,person47};
    account* person49 = new account{{{"Mekdes","Haile","Tadesse"},{5,7,1994},{"Ethiopia","Addis Ababa","Arada",3,4123},"female"},10000049,person48};
    account* person50 = new account{{{"Fikadu","Tesfaye","Hailemariam"},{16,11,1985},{"Ethiopia","Addis Ababa","Bole",4,4234},"male"},10000050,person49};

    return person50;
}