#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <vector>
#include <fstream>
using namespace std;
string const AddDataClient = "Client.txt";
struct StClients {
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance;
    bool MarkForDelete = false; // flag to delete Account 
};
StClients CurrentAccount;
enum enATMOption {
    QuickWithdrow =1 , NormalWithdraw =2 , Deposit =3 , CheckBalance=4 ,LoginOut=5 
};
void ShowATMScreen();
void  NormalWithDrawScreen();
void QuickWithdrawScreen();
void Login();
vector <string> SplitString(string st, string Delim) {

    short pos = 0;
    short Counter = 0;
    string sWord;
    vector <string> vString;
    while ((pos = st.find(Delim)) != (std::string::npos)) {
        sWord = st.substr(0, pos);
        if (sWord != "") {
            vString.push_back(sWord);
        }
        st.erase(0, pos + Delim.length());
    }
    if (sWord != "") {
        vString.push_back(st);
    }
    return vString;
}
string TransfromStructToOneLineString(StClients Client, string Separator = "#//#") {
    string OneLineRecord = "";
    OneLineRecord += Client.AccountNumber + Separator;
    OneLineRecord += Client.PinCode + Separator;
    OneLineRecord += Client.Name + Separator;
    OneLineRecord += Client.Phone + Separator;
    OneLineRecord += to_string(Client.AccountBalance);
    return OneLineRecord;
}
StClients TransformOneLineToStruct(string Line) {
    StClients Clinet;
    vector <string> vClient = SplitString(Line, "#//#");
    Clinet.AccountNumber = vClient[0];
    Clinet.PinCode = vClient[1];
    Clinet.Name = vClient[2];
    Clinet.Phone = vClient[3];
    Clinet.AccountBalance = stod(vClient[4]);

    return Clinet;
}

vector <StClients> LoadClientFileToVector(string FileName) {
    vector <StClients>vClient;
    string Line;
    StClients Client;
    fstream Myfile;
    Myfile.open(FileName, ios::in);//ReadMode 
    if (Myfile.is_open()) {
        while (getline(Myfile, Line)) {//Read File Line By Line AND Save it In the String 
            //1 Load one Line in the to File to Struct 
            Client = TransformOneLineToStruct(Line);
            //2 Load Client Struct in the vector USING While Loop     
            vClient.push_back(Client);
        }
        Myfile.close();
    }
    //3 return the vector 
    return vClient;
}
bool FindAccountNumberAndPinCode(string AccountNumber, string PinCode, StClients& Account) {
    vector <StClients>vClient = LoadClientFileToVector(AddDataClient);
    for (StClients U : vClient) {
        if (U.AccountNumber == AccountNumber && U.PinCode == PinCode) {
            Account = U;
            return true;
        }
    }
    return false;
}
bool LoadAccountNumber(string AccountNumber, string PinCode) {
    if (FindAccountNumberAndPinCode(AccountNumber, PinCode, CurrentAccount))
        return true;
    else
        return false; 

}
vector <StClients> SavaNewDataToFile(string FileName, vector<StClients>vClient) {
    string Line;
    fstream Myfile;
    Myfile.open(FileName, ios::out);//OverWrite 
    if (Myfile.is_open()) {
        for (StClients C : vClient) {
            if (C.MarkForDelete == false) {
                Line = TransfromStructToOneLineString(C);
                Myfile << Line << endl;

            }
        }
        Myfile.close();
    }
    return vClient;
}
bool DepositTransaction(string AccountNumber, double Amount, vector <StClients>& vClient)
{
    char Answer = 'n';

    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (StClients& C : vClient)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
               SavaNewDataToFile(AddDataClient, vClient);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}
short ReadATMOption() {
    short Choice = 0;
    cout << "\nChoose the Option you want [1, 5] ? ";
    cin >> Choice;
    return Choice;
}
short ReadQuickWithdrawAnswer() {
    short Choice = 0;
    cout << "\nChoose the Option you want [1, 9] ? ";
    cin >> Choice;
    return Choice;
}
void GoBackToMainATMMenue() {
    cout << "\n\nPress Any Key To Go Back To Main ATM Menue.....";
    system("pause>0");
    ShowATMScreen(); 
}

double ReadWithDrawAmount() {
    int Amount = 0;
    cout << "\nEnter Amount Multiple of 5's :";
    cin >> Amount;
    while (Amount % 5 != 0) {
        cout << "\nEnter Amount Multiple of 5's :";
        cin >> Amount;
    }
    return  Amount;
}
double ReadDepositAmount() {
    double Amount = 0;
    cout << "\nEnter Positive Deposit Amount :";
    cin >> Amount;
    while (Amount <= 0) {
        cout << "\nEnter Positive Deposit Amount :";
        cin >> Amount;
    }
    return  Amount;
}

short ReadQuickWithdrawChoice()
{
    short Choice = 0;
    cout << "\nChoice Option 1 to 9 :";
    cin >> Choice;
    while (Choice < 1 || Choice >9) {
        cout << "\nChoice Option 1 to 9 :";
        cin >> Choice;
    }
    return Choice;
}
short getQuickWithdrawAmount(short QuickWithDrawChoice) {
    switch (QuickWithDrawChoice) {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0; 

    }
}
void PrefomQuickWithDrawOption(short QuickWithDrawChoice) {
    if (QuickWithDrawChoice == 9)// 
        return;                  //  That Mean Exit



    short WithDrawBalance = getQuickWithdrawAmount(QuickWithDrawChoice);
    if (WithDrawBalance > CurrentAccount.AccountBalance) {
        cout << "Amount exceeds your balance , make another choice \n";
        cout << "Press Any Go Back To Normal Withdraw...... ";
        system("pause>0");
        QuickWithdrawScreen();
        return;
    }
    vector <StClients> vClient = LoadClientFileToVector(AddDataClient);
    DepositTransaction(CurrentAccount.AccountNumber, WithDrawBalance * -1, vClient);
    CurrentAccount.AccountBalance -= WithDrawBalance;
    }


void QuickWithdrawScreen() {
    
    
    system("cls");
    cout << "\t\t\t____________________________\n";
    cout << "\t\t\t   Quick Withdraw Screen  ";
    cout << "\n\t\t\t___________________________\n";
    cout << "\t\t\t[1] 20\t\t[2] 50      \n";
    cout << "\t\t\t[3] 100\t\t[4] 200      \n";
    cout << "\t\t\t[5] 400\t\t[6] 600      \n";
    cout << "\t\t\t[7] 800\t\t[8] 1000      \n";
    cout << "\t\t\t[9] Exit     \n";
    cout << "==============================================\n";
    cout << "Your Balance is" << CurrentAccount.AccountBalance << endl;
    PrefomQuickWithDrawOption(ReadQuickWithdrawChoice());



    



}
void PerfromNormalWithdrawOption() {
    int Amount = ReadWithDrawAmount();
    if (Amount > CurrentAccount.AccountBalance) {
        cout << "Amount exceeds your balance , make another choice \n";
        cout << "Press Any Go Back To Normal Withdraw...... ";
        NormalWithDrawScreen();
        return;
    }
    //here will update in the folder
    vector <StClients> vClient = LoadClientFileToVector(AddDataClient);
    DepositTransaction(CurrentAccount.AccountNumber, Amount * -1, vClient);
    //=====================================================



    //here will update in the Current Account 
    CurrentAccount.AccountBalance -= Amount;
}
void NormalWithDrawScreen() {
    system("cls");
    cout << "\t\t\t____________________________\n";
    cout << "\t\t\t   Normal Withdraw Screen  ";
    cout << "\n\t\t\t___________________________\n";
    
    PerfromNormalWithdrawOption();
    
}


void PerfromDepositOption(){
    double Amount = ReadDepositAmount();

    //here will update in the folder
    vector <StClients> vClient = LoadClientFileToVector(AddDataClient);
    DepositTransaction(CurrentAccount.AccountNumber, Amount, vClient);
    //=====================================================
    


    //here will update in the Current Account 
    CurrentAccount.AccountBalance += Amount;
}

void DepositeScreen() {
    
    system("cls");
    cout << "\t\t\t____________________________\n";
    cout << "\t\t\t   Deposit Screen  ";
    cout << "\n\t\t\t___________________________\n";
    PerfromDepositOption();
    
  
   
}
void CheckBalanceScreen() {
    system("cls");
    cout << "\t\t\t____________________________\n";
    cout << "\t\t\t   CheckBalanceScreen  ";
    cout << "\n\t\t\t___________________________\n";
    cout << "\n\t\t\tYour Balance Is" << CurrentAccount.AccountBalance << endl;
   
}
void PerformATMSystem(enATMOption  ATMOption) {
    switch (ATMOption) {
    case enATMOption::QuickWithdrow:
    {
        system("cls");
        QuickWithdrawScreen();
        GoBackToMainATMMenue(); 
        break;
    }

    case enATMOption::NormalWithdraw:
    {
        system("cls");
        NormalWithDrawScreen();
        GoBackToMainATMMenue();
        break;
    }
    case enATMOption::Deposit:
    {
        system("cls");
        DepositeScreen();
        GoBackToMainATMMenue();
        break;
    }
    case enATMOption::CheckBalance:
    {
        system("cls");
        CheckBalanceScreen();
        GoBackToMainATMMenue();
        break;
    }

    case enATMOption::LoginOut:
    {
        system("Cls");
        Login();
       
        break;
    }

    }
}
short ReadATMMenue() {
    short Choice = 0;
    cout << "\nChoose the Option you want [1, 5] ? ";
    cin >> Choice;
    return Choice;
}
void ShowATMScreen() {
    system("cls");
    cout << "\t\t\t____________________________\n";
    cout << "\t\t\t   ATM Main Menue Screen  ";
    cout << "\n\t\t\t___________________________\n";
    cout << "\t\t\t[1] Quick Withdraw  \n";
    cout << "\t\t\t[2] Normal Withdraw  \n";
    cout << "\t\t\t[3] Deposit   \n";
    cout << "\t\t\t[4] Check Balance \n";
    cout << "\t\t\t[5] Login Out \n";
    PerformATMSystem((enATMOption)ReadATMMenue());
}


void  Login() {
    bool LoginFailed = false;
    string AccountNumber, PinCode;
    do {
        if (LoginFailed) {
            cout << "\n============================\n";
            cout << "Invalid AccountNumber / PinCode \n";
            cout << "\n============================\n";
        }
        cout << "Enter Account Number  :";
        cin >> AccountNumber; 
        cout << "Enter PinCode     :";
        cin >> PinCode;
        LoginFailed   =  (! LoadAccountNumber(AccountNumber, PinCode));
    } while (LoginFailed);
    ShowATMScreen(); 
}





int main()
{

    Login();
}

