#include "classes.h"
#include "sqlite3.h"
#include <map>
#include <vector>
#include <iomanip>
using namespace std ;

// sql functions

string maxId = "0" , typeOfAccount ; // to get the type of the bank account and the max id
map<int , pair< client * , bankAccount *>> bankDataBasic ; // store the basic accounts data
map<int , pair< client * , savingBankAccount *>> bankDataSaving ; // store the saving account data

// creat the database if not exist
static int creatDataBase (char * file) {
    sqlite3 * db ;
    sqlite3_open(file , &db) ;
    sqlite3_close(db) ;
    return 0 ;
}

// creat a table named client if not exist
static int creatTable (char * file) {
    sqlite3 * db ;
    sqlite3_open(file , &db) ;
    string sql = "CREATE TABLE IF NOT EXISTS CLIENT ("
                 "ACCOUNT_TYPE TEXT NOT NULL ,"
                 "initial_ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "NAME TEXT NOT NULL , "
                 "ID TEXT NOT NULL ,"
                 "ADDRESS TEXT NOT NULL ,"
                 "PHONE_NUMBER TEXT NOT NULL ,"
                 "MINIMUM_BALANCE DOUBLE NOT NULL ,"
                 "BALANCE DOUBLE NOT NULL);";
    sqlite3_exec(db , sql.c_str() , NULL , NULL , NULL) ;
    return 0 ;
}

// insert a new row in the table
static int insert (char * file , string data){
    sqlite3 * db ;
    sqlite3_open(file , &db) ;
    string sql = "INSERT INTO CLIENT (ACCOUNT_TYPE , NAME , ID , ADDRESS , PHONE_NUMBER , MINIMUM_BALANCE , BALANCE)"
                 "VALUES "+data+" ;";
    sqlite3_exec(db , sql.c_str() , NULL , NULL , NULL) ;
    return 0 ;
}


// get the data and store it in the maps
static int  callback (void * NotUsed , int argc , char ** arvg , char ** azColName){
    typeOfAccount = arvg[0];
    string id ;
    client * customer = new client ;
    if (typeOfAccount == "BASIC"){
        bankAccount * Account = new bankAccount ;
        for (int i = 1 ; i < argc ; i++){
            if (i == 1){
                id = arvg[i] ;
                maxId = arvg[i];
            }
            if (i == 2){
                customer->setName(arvg[i]) ;
            }
            if (i == 3){
                Account->setID(arvg[i]) ;
            }
            if (i == 4){
                customer->setAddress(arvg[i]) ;
            }
            if (i == 5){
                customer->setPhone(arvg[i]) ;
            }
            if (i == 7){
                string balance = arvg[i] ;
                double Dbalance = stod(balance) ;
                Account->setBalance(Dbalance) ;
            }
        }
        bankDataBasic[stoi(id)] = {customer , Account} ;
    }else {
        savingBankAccount * Account = new savingBankAccount;
        for (int i = 1 ; i < argc ; i++){
            if (i == 1){
                id = arvg[i] ;
                maxId = arvg[i] ;
            }
            if (i == 2){
                customer->setName(arvg[i]) ;
            }
            if (i == 3){
                Account->setID(arvg[i]) ;
            }
            if (i == 4){
                customer->setAddress(arvg[i]) ;
            }
            if (i == 5){
                customer->setPhone(arvg[i]) ;
            }
            if (i == 6){
                string minimumBalance = arvg[i] ;
                double DminimumBalance = stod(minimumBalance) ;
                Account->setMinimumBalance(DminimumBalance) ;
            }
            if (i == 7){
                string balance = arvg[i] ;
                double Dbalance = stod(balance) ;
                Account->setBalance(Dbalance) ;
            }
        }
        bankDataSaving[stoi(id)] = {customer , Account} ;
    }
    return 0 ;
}

// load the data to the map using the call back function
static int load (char * file){
    sqlite3 *db ;
    sqlite3_open(file , &db) ;
    string sql = "SELECT * FROM CLIENT;" ;
    sqlite3_exec(db , sql.c_str() , callback , NULL , NULL);
    return 0 ;
}

// clear the table client from the database
static int clear (char * file){
    sqlite3 * db ;
    sqlite3_open(file , &db) ;
    string sql = "DROP TABLE CLIENT;" ;
    sqlite3_exec(db , sql.c_str() , NULL , NULL , NULL) ;
    return 0 ;
}

/*--------------------------------------------------------------------------*/

// bank Account

bankAccount::bankAccount() {
    balance = 0 ;
}
bankAccount::bankAccount(double bal) {
    balance = bal ;
}
void bankAccount::setBalance(double bal) {
    balance = bal ;
}
void bankAccount::setID(string id) {
    accountID = id ;
}
string bankAccount :: getID (){
    return accountID ;
}
double bankAccount :: getBalance () {
    return balance ;
}
bool bankAccount ::checkWithdraw() {
    return Withdraw ;
}
void bankAccount :: withdraw (double money) {
    if (money <= balance){
        balance -= money ;
        Withdraw = false ;
    }else {
        Withdraw = true ;
    }
}
void bankAccount :: deposit (double money) {
    balance += money ;
}

/*-------------------------------------------------------------------*/
// Saving Bank Account
savingBankAccount::savingBankAccount() {} ;
savingBankAccount :: savingBankAccount(double bal , double mbal) {

    if (bal >= mbal) {
        balance = bal ;
        minimalBalance = mbal ;
    }
    else {
        string answer ;
        cout << "The balance should be bigger than or equal"
                "the minimum balance\n"
             << "So we set the initial value and the minimum value by 1000 L.E\n" ;
    }
}
void savingBankAccount :: setBalance (double bal) {
    if (bal >= minimalBalance) {
        balance = bal;
        checkTheBalance = false ;
    }
    else
        checkTheBalance = true ;
}
double savingBankAccount ::setMinimumBalance(double mbal) {
    minimalBalance = mbal ;
}
double savingBankAccount :: getBalance () {
    return balance ;
}
double savingBankAccount ::getMinimumBalance() {
    return minimalBalance ;
}
void savingBankAccount :: withdraw (double money) {
    if (balance - money < minimalBalance){
        Withdraw = true ;
    }else {
        Withdraw = false ;
        balance -= money ;
    }
}
bool savingBankAccount ::depositCheck() {
    return checkDepiste;
}
bool savingBankAccount ::checkWithdraw() {
    return Withdraw ;
}
void savingBankAccount :: deposit (double money) {
    if (money >= 100){
        balance += money ;
        checkDepiste = false ;
    }else {
        checkDepiste = true ;
    }
}


/*-----------------------------------------------------------------------*/
// Client

client :: client() {}
client :: client (string NAME, string ADDRESS , string PHONE) {
    name = NAME ;
    address = ADDRESS ;
    phoneNumber = PHONE ;
}
void client:: setName (string NAME){
    name = NAME ;
}
void client :: setPhone (string PHONE){
    phoneNumber = PHONE ;
}
void client :: setAddress (string ADDRESS){
    address = ADDRESS ;
}
string client :: getName (){
    return name ;
}
string client :: getPhone (){
    return phoneNumber ;
}
string client :: getAddress (){
    return address ;
}

/*-----------------------------------------------------------------*/


// bank application
int bankApplication ::getOperation() {
    return operation ;
}

void bankApplication ::menu() {
    cout << "Welcome to FCAI Banking Application\n" ;
    cout << "1.Creat a new Account\n" ;
    cout << "2.List Clients and Accounts\n" ;
    cout << "3.withdraw\n" ;
    cout << "4.Deposit\n" ;
    cout << "0.Exit\n" ;
    cout << "Enter your choice\n" ;
    string choice ;
    cin >> choice ;
    bool chechTheChoice = true;
    for (auto i : choice){
        if (!isdigit(i))
            chechTheChoice = false ;
    }
    if (chechTheChoice) {
        int operationNum = stoi(choice);
        if (operationNum >= 0 && operationNum <= 4){
            operation = operationNum ;
        }else {
            cout << "Wrong choice\n" ;
        }
    }else {
        cout << "Wrong choice\n" ;
        operation = 5 ;
    }
}
int bankApplication :: checkId (string id){
    int numId = 0 ;
    if (id[0] == 'F' && id[1] == 'C' && id[2] == 'A' && id[3] == 'I' && id[4] == '-'){
        id.erase(0 , 5) ;
        int countOfZeros = 0 ;
        while (!id.empty()){
            if (id[0] == '0') {
                id.erase(0 , 1);
                countOfZeros ++ ;
            }
            else
                break;
        }
        if (!id.empty()){
            bool checkID = true ;
            for (int i = 0 ; i <id.length() ; i++){
                if (!isdigit(id[i]))
                    checkID = false ;
            }
            if (checkID){
                if ((id.length() == 1 && countOfZeros == 2) ||
                    (id.length() == 2 && countOfZeros == 1) ||
                    (id.length() >= 3 && countOfZeros == 0)) {
                    numId = stoi(id) ;
                }else {
                    cout << "Wrong ID\n"
                         << "You will be returned to the main menu\n\n" ;
                }
            }else {
                cout << "Wrong ID\n"
                     << "You will be returned to the main menu\n\n" ;
            }
        }else {
            cout << "Wrong ID\n"
                 << "You will be returned to the main menu\n\n" ;
        }
    }else {
        cout << "Wrong ID\n"
             << "You will be returned to the main menu\n\n" ;
    }
    return numId ;
}
void bankApplication ::applyOperation() {
    creatDataBase("dataBase.db") ;
    creatTable("dataBase.db");
    if (operation == 1){
        client Client ;
        string name , address , phone , typeOfAccount ;
        double balance , minimumBalance ;
        cout << "Please Enter Client Name\n" ;
        cin.ignore() ;
        getline(cin , name) ;
        cout << "Please Enter Client Address\n" ;
        getline(cin , address) ;
        cout << "Please Enter Client Phone\n" ;
        cin >> phone ;
        Client.setName(name) ;
        Client.setName(address) ;
        Client.setPhone(phone) ;


        // Client handled
        cout << "What Type of Account Do You Like ?\n"
             << "1.Basic\n"
             << "2.Saving\n" ;
        cin >> typeOfAccount ;
        while (typeOfAccount != "1" && typeOfAccount != "2"){
            cout << "Wrong selection\n" ;
            cout << "What Type of Account Do You Like ?\n"
                 << "1.Basic\n"
                 << "2.Saving\n" ;
            cin >> typeOfAccount ;
        }
        if (typeOfAccount == "1"){
            bankAccount Account ;
            cout << "Please Enter the the Starting Balance\n" ;
            cin >> balance ;
            Account.setBalance(balance) ;
        }
        else if (typeOfAccount == "2"){
            savingBankAccount Account ;
            cout << "Please Enter the the Minimum Balance\n" ;
            cin >> minimumBalance ;
            Account.setMinimumBalance(balance) ;
            cout << "Please Enter the balance and it "
                 << "should be greater than or equal the minimumBalance which is " << minimumBalance << "\n" ;
            cin >> balance ;
            while (balance < minimumBalance){
                cout << "The balance is less than the minimum balance\n"
                     << "Please Enter the balance and it "
                     << "should be greater than or equal the minimumBalance which is " << minimumBalance << "\n" ;
                cin >> balance ;
            }
        }
        string ID = "FCAI-" ;
        if (idNum < 10){
            ID += "00" + to_string(idNum) ;
        }else if (idNum < 100){
            ID += "0" + to_string(idNum) ;
        }else {
            ID += to_string(idNum) ;
        }
        idNum ++ ;
        creatDataBase("dataBase.db") ;
        creatTable("dataBase.db") ;
        if (typeOfAccount == "1"){
            typeOfAccount = "BASIC" ;
            string data = "( '" + typeOfAccount +"' , '" + name + " ' , '" + ID + " ' , '" + address + "' , '" + phone + "' , '" + to_string(0) + "' , '" + to_string(balance) + "')" ;
            insert("dataBase.db" , data) ;
        }
        if (typeOfAccount == "2"){
            typeOfAccount = "SAVING" ;
            string data = "( '" + typeOfAccount +"' , '" + name + " ' , '" + ID + " ' , '" + address + "' , '" + phone + "' , '" + to_string(minimumBalance) + "' , '" + to_string(balance) + "')" ;
            insert("dataBase.db" , data) ;
        }
        cout << "An Account was created with ID " << ID << " and Starting Balance "<< balance << " L.E\n"
             << "Please Save your ID you will need it if you want to withdraw or deposit\n\n" ;
        // creat data to store in the database if not created
    }
    if (operation == 2){
        // list clients from database
        int count = 1 ;
        while (count < idNum ){
            if (bankDataBasic[count].first != 0){
                cout <<setw(10) << setfill ('-')<< bankDataBasic[count].first->getName() <<setw(10) << setfill ('-') <<  "\n"
                     << "Address : "   << bankDataBasic[count].first->getAddress() << "\n"
                     << "Phone :"      << bankDataBasic[count].first->getPhone() << "\n"
                     << "Account ID : "<< bankDataBasic[count].second->getID() << " ( basic )\n"
                     << "Balance : "   << bankDataBasic[count].second->getBalance() << "\n" ;
            }else {
                cout <<setw(10) << setfill ('-')<< bankDataSaving[count].first->getName() <<setw(10) << setfill ('-') <<  "\n"
                     << "Address : "   << bankDataSaving[count].first->getAddress() << "\n"
                     << "Phone :"      << bankDataSaving[count].first->getPhone() << "\n"
                     << "Account ID : "<< bankDataSaving[count].second->getID() << " ( saving )\n"
                     << "Balance : "   << bankDataSaving[count].second->getBalance() << "\n"
                     << "Minimum balance : " << bankDataSaving[count].second->getMinimumBalance() << "\n";
            }
            count ++ ;
        }
        if (count == 1){
            cout << "No customers are in the database\n" ;
        }
        cout << "\n" ;
    }
    if (operation == 3){
        // withdraw
        string id ;
        cout << "Enter your Account ID (e.g , FCAI-015) \n" ;
        cin >> id ;
        int numId = checkId(id) ;
        if (numId != 0){
            double withdraw ;
            if (bankDataBasic[numId].first != 0){
                cout << "mr/ " << bankDataBasic[numId].first->getName() << "\n"
                     << "Your data is \n"
                     << "Account Id : " << id << "\n"
                     << "Account Type : Basic\n"
                     << "Balance : " << bankDataBasic[numId].second->getBalance() << "\n" ;
                cout << "Enter the amount of withdraw\n" ;
                cin >> withdraw ;
                bankDataBasic[numId].second->withdraw(withdraw) ;
                while (bankDataBasic[numId].second->checkWithdraw()){
                    string answer ;
                    cout << "Sorry. This is more than you have in your account\n"
                         << "Do you want to re-withdraw again\n"
                         << "1.Yes\n2.No\n" ;
                    cin >> answer ;
                    if (answer != "1" && answer != "2"){
                        cout << "Wrong choice\n" ;
                    }else {
                        if (answer == "1"){
                            cout << "Enter the amount of withdraw\n" ;
                            cin >> withdraw ;
                            bankDataBasic[numId].second->withdraw(withdraw) ;
                        }else if (answer == "2"){
                            cout << "Ok You will be returned to the main menu\n"
                                 << "Thank you For using mr/ " << bankDataBasic[numId].first->getName() << " for using\n\n" ;
                            return;
                        }
                    }
                }
                cout << "Thank You mr / "<<bankDataBasic[numId].first->getName() <<" for using\nAccount : ID "
                     << id << "\nNew Balance : " << bankDataBasic[numId].second->getBalance() << "\n\n" ;

                // update the data after withdraw
                updateData("dataBase.db") ;
            }else if (bankDataSaving[numId].first != 0){
                cout << "mr/ " << bankDataSaving[numId].first->getName() << "\n"
                     << "Your data is \n"
                     << "Account Id : " << id << "\n"
                     << "Account Type : Saving\n"
                     << "Balance : " << bankDataSaving[numId].second->getBalance() << "\n"
                     << "Minimum Balance : " << bankDataSaving[numId].second->getMinimumBalance() << "\n" ;
                cout << "Enter the amount of withdraw\n" ;
                cin >> withdraw ;
                bankDataSaving[numId].second->withdraw(withdraw) ;
                while (bankDataSaving[numId].second->checkWithdraw()){
                    string answer ;
                    cout << "Sorry. This is more than you can withdraw\n"
                         << "The withdraw amount shouldn't make the balance less than the minimum balance\n"
                         << "Which is : " << bankDataSaving[numId].second->getMinimumBalance() << "\n"
                         << "Do you want to re-withdraw again\n"
                         << "1.Yes\n2.No\n" ;
                    cin >> answer ;
                    if (answer != "1" && answer != "2"){
                        cout << "Wrong choice\n" ;
                    }else {
                        if (answer == "1"){
                            cout << "Enter the amount of withdraw\n" ;
                            cin >> withdraw ;
                            bankDataSaving[numId].second->withdraw(withdraw) ;
                        }else if (answer == "2"){
                            cout << "Ok You will be returned to the main menu\n"
                                 << "Thank you For using mr/ " << bankDataSaving[numId].first->getName() << " for using\n\n" ;
                            return;
                        }
                    }
                }
                cout << "Thank You mr / "<<bankDataSaving[numId].first->getName() <<" for using\nAccount : ID "
                     << id << "\nNew Balance : " << bankDataSaving[numId].second->getBalance() << "\n\n" ;

                // update the data after withdraw
                updateData("dataBase.db") ;
            }else
                cout << "Not Exist ID\n"
                     << "You will be returned to the main menu\n"
                     << "Thank you for using \n\n" ;
        }
    }
    if (operation == 4){
        string id ;
        cout << "Enter your Account ID (e.g , FCAI-015) \n" ;
        cin >> id ;
        int numId = checkId(id) ;
        if (numId != 0){
            double deposit ;
            if (bankDataBasic[numId].first != 0){
                cout << "mr/ " << bankDataBasic[numId].first->getName() << "\n"
                     << "your data is\n"
                     << "Account Id : " << id << "\n"
                     << "Account Type : Basic\n"
                     << "Balance : " << bankDataBasic[numId].second->getBalance() << "\n" ;
                cout << "Enter the amount of deposit\n" ;
                cin >> deposit ;
                bankDataBasic[numId].second->deposit(deposit) ;
                // update the data after deposit
                updateData("dataBase.db") ;
                cout << "Thank You mr/ "<< bankDataBasic[numId].first->getName() <<" for using\nAccount : ID "
                     << id << "\nNew Balance : " << bankDataBasic[numId].second->getBalance() << "\n\n" ;
            }else if (bankDataSaving[numId].first != 0){
                cout << "mr/ " << bankDataSaving[numId].first->getName() << "\n"
                     << "your data is\n"
                     << "Account Id : " << id << "\n"
                     << "Account Type : Saving\n"
                     << "Balance : " << bankDataSaving[numId].second->getBalance() << "\n"
                     << "Minimum Balance : " << bankDataSaving[numId].second->getMinimumBalance() << "\n" ;
                cout << "Enter the amount of deposit\n" ;
                cin >> deposit ;
                bankDataSaving[numId].second->deposit(deposit) ;
                while (bankDataSaving[numId].second->depositCheck()){
                    string answer ;
                    cout << "The minimum amount to deposit is at least 100 L.E\n"
                         << "Do you want to deposit again\n"
                         << "1.Yes\nNo\n" ;
                    cin >> answer ;
                    while (answer != "1" && answer != "2"){
                        cout << "Wrong Choice\n" ;
                        cout << "Do you want to re-deposit again\n"
                             << "1.Yes\nNo\n" ;
                        cin >> answer ;
                    }
                    if (answer == "1"){
                        cout << "Enter the amount of deposit\n" ;
                        cin >> deposit ;
                        bankDataSaving[numId].second->deposit(deposit) ;
                    }else if (answer == "2"){
                        cout << "Thank you mr/ " << bankDataSaving[numId].first->getName() << " for Using\n\n" ;
                        return;
                    }
                }
                // update the data after deposit
                updateData("dataBase.db") ;
                cout << "Thank You mr/ "<< bankDataSaving[numId].first->getName()<<" For using\nAccount : ID "
                     << id << "\nNew Balance : " << bankDataSaving[numId].second->getBalance() << "\n\n" ;
            }else
                cout << "Not Exist ID\n"
                     << "You will be returned to the main menu\n\n" ;
        }
    }
    if (operation != 5 && operation != 0){
        cout << setw(30) << setfill('-') << "\n\n" ;
    }
}

void bankApplication::updateData(char * file) {
    clearData(file);
    creatDataBase(file) ;
    creatTable(file) ;
    int count = 1 ;
    while (count < idNum){
        if (bankDataBasic[count].first != 0){
            string data = "('BASIC' , '" + bankDataBasic[count].first->getName()
                    + "' , '" + bankDataBasic[count].second->getID()
                    + "' , '" + bankDataBasic[count].first->getAddress()
                    + "' , '" + bankDataBasic[count].first->getPhone()
                    + "' , '" + to_string(0) +
                    + "' , '" + to_string(bankDataBasic[count].second->getBalance()) + "')" ;
            insert(file , data) ;
        }else {
            string data = "('SAVING' , '" + bankDataSaving[count].first->getName()
                          + "' , '" + bankDataSaving[count].second->getID()
                          + "' , '" + bankDataSaving[count].first->getAddress()
                          + "' , '" + bankDataSaving[count].first->getPhone()
                          + "' , '" + to_string(bankDataSaving[count].second->getMinimumBalance()) +
                          + "' , '" + to_string(bankDataSaving[count].second->getBalance()) + "')" ;
            insert(file , data) ;
        }
        count ++ ;
    }
}

void bankApplication ::loadData(char * file) {
    load(file) ;
    idNum = max(idNum , stoi(maxId) + 1);
}
void bankApplication :: clearData(char * file){
    clear(file) ;
}