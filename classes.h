#ifndef BANK_SYSSTEM_CLASSES_H
#define BANK_SYSSTEM_CLASSES_H

#include <iostream>
using namespace std ;
class bankAccount {
protected:
    string accountID ;
    double balance ;
    bool Withdraw = false ;
public:
    bankAccount() ;
    bankAccount (double bal) ;
    void setBalance (double bal) ;
    void setID (string id) ;
    bool checkWithdraw();
    string getID ();
    double getBalance () ;
    void withdraw (double money) ;
    void deposit (double money) ;
};
class savingBankAccount: public bankAccount{
protected:
    double minimalBalance = 1000 ;
    bool  checkTheBalance = false ,
          checkDepiste = false  ;

public:
    bool depositCheck() ;
    bool checkWithdraw();
    savingBankAccount() ;
    savingBankAccount(double bal , double mbal) ;
    void setBalance (double bal) ;
    double setMinimumBalance(double mbal) ;
    double getBalance () ;
    double getMinimumBalance () ;
    void withdraw (double money) ;
    void deposit (double money) ;
};
class client {
protected:
    string name , address , phoneNumber ;
public:
    client() ;
    client(string name , string address , string phoneNumber) ;
    void setName (string NAME) ;
    void setPhone (string PHONE) ;
    void setAddress (string ADDRESS);
    string getName ();
    string getPhone ();
    string getAddress ();
};

class bankApplication{
protected:
    int idNum = 1 ;
    int operation = 5 ;
public:
    int getOperation() ;
    void menu () ;
    void applyOperation() ;
    void loadData (char * file) ;
    void updateData (char * file) ;
    void clearData (char * file) ;
    int checkId (string id) ;
};
#endif