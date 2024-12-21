#include "wallet.h"
#include <iostream>

Wallet::Wallet(const string& name, double balance) : name(name), balance(balance) {}

void Wallet::addFunds(double amount) {
    balance += amount;
}

bool Wallet::spend(double amount, const string& category, time_t date) {
    if (balance >= amount) {
        balance -= amount;
        expenses.push_back(Expense(category, amount, name, date));
        return true;
    }
    return false;
}

void Wallet::printExpenses() const {
    for (const auto& expense : expenses) {
        cout << "Category: " << expense.category
            << ", Amount: " << expense.amount
            << ", Date: " << expense.getDate()
            << ", Wallet: " << expense.wallet_name << endl;
    }
}
