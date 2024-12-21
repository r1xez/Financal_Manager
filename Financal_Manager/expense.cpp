#include "expense.h"
#include <iomanip>
#include <sstream>

Expense::Expense(const string& category, double amount, const string& wallet_name, time_t date)
    : category(category), amount(amount), wallet_name(wallet_name), date(date) {}

string Expense::getDate() const {
    char buffer[20];
    struct tm timeinfo;
    localtime_s(&timeinfo, &date);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
    return string(buffer);
}
