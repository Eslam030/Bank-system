#include "classes.h"
int main() {
    bankApplication app ;
//    app.loadData("dataBase.db") ;
//    savingBankAccount aa (120) ;
//    app.clearData() ;
    while (app.getOperation()) {
    app.loadData("dataBase.db") ;
    app.menu();
    app.applyOperation() ;
    }
//    app.updateData("dataBase.db") ;
}
