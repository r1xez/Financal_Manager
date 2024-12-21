#ifndef CARD_H
#define CARD_H

#include <string>
using namespace std;

class Card {
public:
    string name;
    double balance;

    Card(const string& name, double balance);
    void addFunds(double amount);
    bool spend(double amount);
};

#endif
