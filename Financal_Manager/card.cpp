#include "card.h"

Card::Card(const string& name, double balance) : name(name), balance(balance) {}

void Card::addFunds(double amount) {
    balance += amount;
}

bool Card::spend(double amount) {
    if (balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}
