#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <fstream>
#include "card.h"
#include "wallet.h"
#include "expense.h"

using namespace std;

class FinanceManager {
private:
    vector<Wallet> wallets;
    vector<Card> cards;
    vector<Expense> expenses;

    vector<Expense> getTopExpenses(const vector<Expense>& filteredExpenses, int topN) {
        vector<Expense> sortedExpenses = filteredExpenses;
        sort(sortedExpenses.begin(), sortedExpenses.end(), [](const Expense& a, const Expense& b) {
            return a.amount > b.amount;
            });
        if (sortedExpenses.size() > topN) {
            sortedExpenses.resize(topN);
        }
        return sortedExpenses;
    }

    vector<Expense> filterExpensesByPeriod(const string& period) {
        vector<Expense> filteredExpenses;
        time_t now = time(nullptr);

        for (const auto& expense : expenses) {
            double daysDiff = difftime(now, expense.date) / (60 * 60 * 24);

            if ((period == "day" && daysDiff <= 1) ||
                (period == "week" && daysDiff <= 7) ||
                (period == "month" && daysDiff <= 30)) {
                filteredExpenses.push_back(expense);
            }
        }
        return filteredExpenses;
    }

    bool addExpenseFromEntity(const string& name, double amount, const string& category, time_t date) {
        auto findEntity = [&](auto& container, const string& entityName) -> bool {
            for (auto& entity : container) {
                if (entity.name == entityName) {
                    if (entity.spend(amount, category, date)) {
                        expenses.push_back(Expense(category, amount, entity.name, date));
                        return true;
                    }
                    cout << "Not enough funds in " << entityName << "." << endl;
                    return false;
                }
            }
            return false;
        };

        if (findEntity(wallets, name)) {
            return true;
        }

        auto findCard = [&](auto& container, const string& entityName) -> bool {
            for (auto& entity : container) {
                if (entity.name == entityName) {
                    if (entity.spend(amount)) {
                        expenses.push_back(Expense(category, amount, entity.name, date));
                        return true;
                    }
                    cout << "Not enough funds in " << entityName << "." << endl;
                    return false;
                }
            }
            return false;
        };

        if (findCard(cards, name)) {
            return true;
        }

        cout << "Entity with name \"" << name << "\" not found." << endl;
        return false;
    }

public:
    void addCard() {
        string name;
        double balance;
        cout << "Enter the name of the card: ";
        cin >> name;
        cout << "Enter the balance of the card: ";
        cin >> balance;
        cards.push_back(Card(name, balance));
        cout << "Card \"" << name << "\" with balance " << balance << " has been added." << endl;
    }

    void addWallet() {
        string name;
        double balance;
        cout << "Enter the name of the wallet: ";
        cin >> name;
        cout << "Enter the balance of the wallet: ";
        cin >> balance;
        wallets.push_back(Wallet(name, balance));
        cout << "Wallet \"" << name << "\" with balance " << balance << " has been added." << endl;
    }

    void addFunds() {
        string name;
        double amount;
        cout << "Enter the name of the card or wallet: ";
        cin >> name;
        cout << "Enter the amount to add: ";
        cin >> amount;

        auto card = find_if(cards.begin(), cards.end(), [&name](const Card& card) { return card.name == name; });
        if (card != cards.end()) {
            card->addFunds(amount);
            cout << "Added " << amount << " to card \"" << name << "\"." << endl;
            return;
        }

        auto wallet = find_if(wallets.begin(), wallets.end(), [&name](const Wallet& wallet) { return wallet.name == name; });
        if (wallet != wallets.end()) {
            wallet->addFunds(amount);
            cout << "Added " << amount << " to wallet \"" << name << "\"." << endl;
            return;
        }

        cout << "Card or wallet with name \"" << name << "\" not found." << endl;
    }

    void addExpense() {
        string name, category;
        double amount;
        cout << "Enter the name of the wallet or card: ";
        cin >> name;
        cout << "Enter the category of the expense: ";
        cin >> category;
        cout << "Enter the amount of the expense: ";
        cin >> amount;

        if (!addExpenseFromEntity(name, amount, category, time(nullptr))) {
            cout << "Failed to add expense." << endl;
        }
    }

    void printExpenses() const {
        cout << "Expenses for all cards and wallets:" << endl;
        for (const auto& expense : expenses) {
            cout << "Category: " << expense.category
                << ", Amount: " << expense.amount
                << ", Date: " << expense.getDate()
                << ", Wallet/Card: " << expense.wallet_name << endl;
        }
    }

    void generateReport(const string& period) {
        vector<Expense> filteredExpenses = filterExpensesByPeriod(period);

        ofstream reportFile("report.txt");
        if (!reportFile.is_open()) {
            cout << "Failed to create or open the report file." << endl;
            return;
        }

        reportFile << "Expenses for the last " << period << ":" << endl;
        for (const auto& expense : filteredExpenses) {
            reportFile << "Category: " << expense.category << ", Amount: " << expense.amount
                << " UAH, Date: " << expense.getDate() << ", Wallet/Card: " << expense.wallet_name << endl;
        }

        reportFile.close();
        cout << "Report saved in file: report.txt" << endl;
    }

    void generateTop3(const string& period) {
        vector<Expense> filteredExpenses = filterExpensesByPeriod(period);
        vector<Expense> topExpenses = getTopExpenses(filteredExpenses, 3);

        cout << "Top 3 expenses for the last " << period << ":" << endl;
        for (const auto& expense : topExpenses) {
            cout << "Category: " << expense.category
                << ", Amount: " << expense.amount
                << ", Date: " << expense.getDate()
                << ", Wallet/Card: " << expense.wallet_name << endl;
        }
    }
};

int main() {
    FinanceManager manager;
    int choice;

    do {
        cout << "\nFinance Manager Menu:\n";
        cout << "1. Add Card\n";
        cout << "2. Add Wallet\n";
        cout << "3. Add Funds\n";
        cout << "4. Add Expense\n";
        cout << "5. View Expenses\n";
        cout << "6. Generate Report\n";
        cout << "7. Generate Top 3 Expenses\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            manager.addCard();
            break;
        case 2:
            manager.addWallet();
            break;
        case 3:
            manager.addFunds();
            break;
        case 4:
            manager.addExpense();
            break;
        case 5:
            manager.printExpenses();
            break;
        case 6: {
            string period;
            cout << "Enter the period (day, week, month): ";
            cin >> period;
            manager.generateReport(period);
            break;
        }
        case 7: {
            string period;
            cout << "Enter the period (day, week, month): ";
            cin >> period;
            manager.generateTop3(period);
            break;
        }
        case 8:
            cout << "Exiting Finance Manager. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 8);

    return 0;
}
