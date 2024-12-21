#ifndef EXPENSE_H
#define EXPENSE_H

#include <string>
#include <ctime>
using namespace std;

class Expense {
public:
    string category;
    double amount;
    string wallet_name;
    time_t date;

  
    Expense() : category(""), amount(0), wallet_name(""), date(time(nullptr)) {}

    
    Expense(const string& category, double amount, const string& wallet_name, time_t date);

    string getDate() const;
};

#endif
