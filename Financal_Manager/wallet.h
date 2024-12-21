#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>
#include <ctime>
#include "expense.h"

using namespace std;

class Wallet {
public:
    string name;
    double balance;
    vector<Expense> expenses;

    Wallet(const string& name, double balance);
    void addFunds(double amount);
    bool spend(double amount, const string& category, time_t date);
    void printExpenses() const;
};

#endif
