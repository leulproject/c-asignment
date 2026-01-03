#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

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


int main()
{
  person p[50] = {
        {1, {"Abebe", "Kebede", "Yohannes"}, {12, 5, 1990}, {"Ethiopia", "Addis Ababa", "Bole", 3, 12}, "Male", 911234501},
        {2, {"Hirut", "Mekonnen", "Tadesse"}, {23, 7, 1985}, {"Ethiopia", "Adama", "Arada", 5, 22}, "Female", 911234502},
        {3, {"Mekonnen", "Tesfaye", "Gebre"}, {5, 11, 1992}, {"Ethiopia", "Gondar", "Kebele1", 2, 10}, "Male", 911234503},
        {4, {"Lily", "Haile", "Bekele"}, {17, 2, 1988}, {"Ethiopia", "Hawassa", "Kebele3", 6, 8}, "Female", 911234504},
        {5, {"Daniel", "Fikremariam", "Alemu"}, {30, 9, 1995}, {"Ethiopia", "Dire Dawa", "Kebele5", 1, 15}, "Male", 911234505},
        {6, {"Selam", "Gebremedhin", "Yimer"}, {10, 10, 1991}, {"Ethiopia", "Bahir Dar", "Kebele2", 4, 18}, "Female", 911234506},
        {7, {"Tsegaye", "Asfaw", "Mulu"}, {21, 1, 1987}, {"Ethiopia", "Mekelle", "Kebele6", 3, 9}, "Male", 911234507},
        {8, {"Eyerusalem", "Tadesse", "Bekalu"}, {15, 3, 1993}, {"Ethiopia", "Addis Ababa", "Kebele4", 5, 11}, "Female", 911234508},
        {9, {"Samuel", "Berhanu", "Fikadu"}, {8, 12, 1989}, {"Ethiopia", "Adama", "Bole", 2, 6}, "Male", 911234509},
        {10, {"Hana", "Alemu", "Tesfaye"}, {3, 6, 1994}, {"Ethiopia", "Gondar", "Kebele3", 1, 20}, "Female", 911234510},
        {11, {"Getachew", "Yohannes", "Kassahun"}, {19, 8, 1986}, {"Ethiopia", "Hawassa", "Kebele7", 4, 14}, "Male", 911234511},
        {12, {"Selamawit", "Fikadu", "Mengistu"}, {27, 9, 1990}, {"Ethiopia", "Addis Ababa", "Arada", 3, 17}, "Female", 911234512},
        {13, {"Mulugeta", "Bekele", "Haile"}, {1, 2, 1988}, {"Ethiopia", "Bahir Dar", "Kebele5", 2, 8}, "Male", 911234513},
        {14, {"Rahel", "Asfaw", "Yonas"}, {14, 4, 1992}, {"Ethiopia", "Dire Dawa", "Kebele2", 5, 12}, "Female", 911234514},
        {15, {"Binyam", "Tesfaye", "Gebremedhin"}, {25, 7, 1991}, {"Ethiopia", "Mekelle", "Kebele1", 1, 7}, "Male", 911234515},
        {16, {"Abebech", "Mulu", "Bekele"}, {6, 11, 1989}, {"Ethiopia", "Addis Ababa", "Kebele6", 2, 16}, "Female", 911234516},
        {17, {"Yonas", "Fikadu", "Haile"}, {18, 5, 1987}, {"Ethiopia", "Adama", "Bole", 3, 19}, "Male", 911234517},
        {18, {"Lily", "Mengistu", "Kebede"}, {9, 10, 1993}, {"Ethiopia", "Gondar", "Kebele3", 4, 9}, "Female", 911234518},
        {19, {"Tadesse", "Yimer", "Mulu"}, {2, 12, 1990}, {"Ethiopia", "Hawassa", "Kebele4", 5, 13}, "Male", 911234519},
        {20, {"Hanna", "Bekalu", "Asfaw"}, {16, 1, 1992}, {"Ethiopia", "Bahir Dar", "Kebele2", 1, 11}, "Female", 911234520},
        {21, {"Abraham", "Kassahun", "Fikadu"}, {22, 3, 1986}, {"Ethiopia", "Addis Ababa", "Arada", 2, 14}, "Male", 911234521},
        {22, {"Mekdes", "Tesfaye", "Haile"}, {29, 6, 1989}, {"Ethiopia", "Dire Dawa", "Kebele5", 3, 17}, "Female", 911234522},
        {23, {"Solomon", "Yohannes", "Bekele"}, {4, 8, 1991}, {"Ethiopia", "Mekelle", "Kebele6", 4, 8}, "Male", 911234523},
        {24, {"Selam", "Mengistu", "Tadesse"}, {12, 11, 1988}, {"Ethiopia", "Addis Ababa", "Kebele3", 5, 15}, "Female", 911234524},
        {25, {"Bekele", "Asfaw", "Gebremedhin"}, {7, 9, 1990}, {"Ethiopia", "Gondar", "Kebele1", 1, 10}, "Male", 911234525},
        {26, {"Hana", "Mulu", "Bekele"}, {20, 2, 1992}, {"Ethiopia", "Adama", "Bole", 2, 12}, "Female", 911234526},
        {27, {"Mekonnen", "Fikadu", "Haile"}, {30, 12, 1987}, {"Ethiopia", "Bahir Dar", "Kebele5", 3, 19}, "Male", 911234527},
        {28, {"Rahel", "Yimer", "Kebede"}, {11, 5, 1989}, {"Ethiopia", "Addis Ababa", "Arada", 4, 20}, "Female", 911234528},
        {29, {"Samuel", "Tesfaye", "Mengistu"}, {1, 1, 1991}, {"Ethiopia", "Dire Dawa", "Kebele2", 5, 6}, "Male", 911234529},
        {30, {"Lily", "Bekalu", "Asfaw"}, {14, 7, 1993}, {"Ethiopia", "Mekelle", "Kebele6", 1, 13}, "Female", 911234530},
        {31, {"Daniel", "Kassahun", "Fikadu"}, {23, 8, 1990}, {"Ethiopia", "Addis Ababa", "Bole", 2, 8}, "Male", 911234531},
        {32, {"Selam", "Tesfaye", "Haile"}, {6, 3, 1992}, {"Ethiopia", "Gondar", "Kebele3", 3, 17}, "Female", 911234532},
        {33, {"Tadesse", "Yohannes", "Bekele"}, {19, 4, 1988}, {"Ethiopia", "Bahir Dar", "Kebele4", 4, 14}, "Male", 911234533},
        {34, {"Hirut", "Mulu", "Kebede"}, {28, 9, 1991}, {"Ethiopia", "Addis Ababa", "Arada", 5, 9}, "Female", 911234534},
        {35, {"Abraham", "Fikadu", "Mengistu"}, {3, 6, 1989}, {"Ethiopia", "Dire Dawa", "Kebele5", 1, 18}, "Male", 911234535},
        {36, {"Mekdes", "Bekalu", "Haile"}, {15, 2, 1990}, {"Ethiopia", "Mekelle", "Kebele6", 2, 11}, "Female", 911234536},
        {37, {"Solomon", "Tesfaye", "Gebremedhin"}, {9, 10, 1992}, {"Ethiopia", "Addis Ababa", "Bole", 3, 16}, "Male", 911234537},
        {38, {"Selamawit", "Yimer", "Kassahun"}, {21, 11, 1987}, {"Ethiopia", "Gondar", "Kebele1", 4, 12}, "Female", 911234538},
        {39, {"Bekele", "Mengistu", "Tadesse"}, {2, 1, 1988}, {"Ethiopia", "Bahir Dar", "Kebele2", 5, 19}, "Male", 911234539},
        {40, {"Hana", "Asfaw", "Bekele"}, {17, 5, 1991}, {"Ethiopia", "Addis Ababa", "Arada", 1, 10}, "Female", 911234540},
        {41, {"Mekonnen", "Tesfaye", "Haile"}, {26, 7, 1990}, {"Ethiopia", "Dire Dawa", "Kebele5", 2, 13}, "Male", 911234541},
        {42, {"Rahel", "Bekalu", "Kebede"}, {8, 12, 1992}, {"Ethiopia", "Mekelle", "Kebele6", 3, 9}, "Female", 911234542},
        {43, {"Samuel", "Yohannes", "Fikadu"}, {13, 3, 1989}, {"Ethiopia", "Addis Ababa", "Bole", 4, 18}, "Male", 911234543},
        {44, {"Lily", "Mengistu", "Asfaw"}, {24, 9, 1991}, {"Ethiopia", "Gondar", "Kebele3", 5, 15}, "Female", 911234544},
        {45, {"Daniel", "Fikadu", "Haile"}, {5, 6, 1988}, {"Ethiopia", "Bahir Dar", "Kebele4", 1, 14}, "Male", 911234545},
        {46, {"Selam", "Bekalu", "Kassahun"}, {18, 1, 1990}, {"Ethiopia", "Addis Ababa", "Arada", 2, 17}, "Female", 911234546},
        {47, {"Tadesse", "Tesfaye", "Gebremedhin"}, {11, 8, 1987}, {"Ethiopia", "Dire Dawa", "Kebele5", 3, 7}, "Male", 911234547},
        {48, {"Hirut", "Yimer", "Bekele"}, {22, 4, 1992}, {"Ethiopia", "Mekelle", "Kebele6", 4, 16}, "Female", 911234548},
        {49, {"Abraham", "Mengistu", "Tadesse"}, {7, 10, 1989}, {"Ethiopia", "Addis Ababa", "Bole", 5, 11}, "Male", 911234549},
        {50, {"Mekdes", "Asfaw", "Haile"}, {29, 12, 1991}, {"Ethiopia", "Gondar", "Kebele1", 1, 12}, "Female", 911234550}
    };
    account a[50] = {
        {1, 10001, "user1", "pass1"}, {2, 10002, "user2", "pass2"},
        {3, 10003, "user3", "pass3"}, {4, 10004, "user4", "pass4"},
        {5, 10005, "user5", "pass5"}, {6, 10006, "user6", "pass6"},
        {7, 10007, "user7", "pass7"}, {8, 10008, "user8", "pass8"},
        {9, 10009, "user9", "pass9"}, {10, 10010, "user10", "pass10"},
        {11, 10011, "user11", "pass11"}, {12, 10012, "user12", "pass12"},
        {13, 10013, "user13", "pass13"}, {14, 10014, "user14", "pass14"},
        {15, 10015, "user15", "pass15"}, {16, 10016, "user16", "pass16"},
        {17, 10017, "user17", "pass17"}, {18, 10018, "user18", "pass18"},
        {19, 10019, "user19", "pass19"}, {20, 10020, "user20", "pass20"},
        {21, 10021, "user21", "pass21"}, {22, 10022, "user22", "pass22"},
        {23, 10023, "user23", "pass23"}, {24, 10024, "user24", "pass24"},
        {25, 10025, "user25", "pass25"}, {26, 10026, "user26", "pass26"},
        {27, 10027, "user27", "pass27"}, {28, 10028, "user28", "pass28"},
        {29, 10029, "user29", "pass29"}, {30, 10030, "user30", "pass30"},
        {31, 10031, "user31", "pass31"}, {32, 10032, "user32", "pass32"},
        {33, 10033, "user33", "pass33"}, {34, 10034, "user34", "pass34"},
        {35, 10035, "user35", "pass35"}, {36, 10036, "user36", "pass36"},
        {37, 10037, "user37", "pass37"}, {38, 10038, "user38", "pass38"},
        {39, 10039, "user39", "pass39"}, {40, 10040, "user40", "pass40"},
        {41, 10041, "user41", "pass41"}, {42, 10042, "user42", "pass42"},
        {43, 10043, "user43", "pass43"}, {44, 10044, "user44", "pass44"},
        {45, 10045, "user45", "pass45"}, {46, 10046, "user46", "pass46"},
        {47, 10047, "user47", "pass47"}, {48, 10048, "user48", "pass48"},
        {49, 10049, "user49", "pass49"}, {50, 10050, "user50", "pass50"}
    };
    moneyfile m[50] = {
        {1, 1234.56, 1685000000}, {2, 7890.12, 1685086400}, {3, 3456.78, 1685172800},
        {4, 9012.34, 1685259200}, {5, 5678.90, 1685345600}, {6, 2345.67, 1685432000},
        {7, 8901.23, 1685518400}, {8, 4567.89, 1685604800}, {9, 123.45, 1685691200},
        {10, 6789.01, 1685777600}, {11, 234.56, 1685864000}, {12, 789.01, 1685950400},
        {13, 345.67, 1686036800}, {14, 901.23, 1686123200}, {15, 567.89, 1686209600},
        {16, 234.12, 1686296000}, {17, 890.45, 1686382400}, {18, 456.78, 1686468800},
        {19, 123.89, 1686555200}, {20, 678.34, 1686641600}, {21, 234.90, 1686728000},
        {22, 789.56, 1686814400}, {23, 345.12, 1686900800}, {24, 901.45, 1686987200},
        {25, 567.34, 1687073600}, {26, 234.78, 1687160000}, {27, 890.12, 1687246400},
        {28, 456.90, 1687332800}, {29, 123.56, 1687419200}, {30, 678.12, 1687505600},
        {31, 234.45, 1687592000}, {32, 789.34, 1687678400}, {33, 345.90, 1687764800},
        {34, 901.56, 1687851200}, {35, 567.12, 1687937600}, {36, 234.89, 1688024000},
        {37, 890.34, 1688110400}, {38, 456.12, 1688196800}, {39, 123.78, 1688283200},
        {40, 678.56, 1688369600}, {41, 234.34, 1688456000}, {42, 789.12, 1688542400},
        {43, 345.45, 1688628800}, {44, 901.78, 1688715200}, {45, 567.12, 1688801600},
        {46, 234.56, 1688888000}, {47, 890.90, 1688974400}, {48, 456.78, 1689060800},
        {49, 123.12, 1689147200}, {50, 678.45, 1689233600}
    };
   
    
    // Write person records to person.bank
    {
        std::ofstream pf("person.bank", std::ios::binary | std::ios::out | std::ios::trunc);
        if (!pf) {
            cerr << "Failed to open person.bank for writing\n";
        } else {
            for (int i = 0; i < 50; ++i) {
                pf.write(reinterpret_cast<const char*>(&p[i]), sizeof(person));
            }
            pf.close();
            cout << "Wrote 50 person records to person.bank\n";
        }
    }

    // Write account records to account.bank
    {
        std::ofstream af("account.bank", std::ios::binary | std::ios::out | std::ios::trunc);
        if (!af) {
            cerr << "Failed to open account.bank for writing\n";
        } else {
            for (int i = 0; i < 50; ++i) {
                af.write(reinterpret_cast<const char*>(&a[i]), sizeof(account));
            }
            af.close();
            cout << "Wrote 50 account records to account.bank\n";
        }
    }

    // Write money records to money.bank
    {
        std::ofstream mf("money.bank", std::ios::binary | std::ios::out | std::ios::trunc);
        if (!mf) {
            cerr << "Failed to open money.bank for writing\n";
        } else {
            for (int i = 0; i < 50; ++i) {
                mf.write(reinterpret_cast<const char*>(&m[i]), sizeof(moneyfile));
            }
            mf.close();
            cout << "Wrote 50 money records to money.bank\n";
        }
    }

    return 0;
}