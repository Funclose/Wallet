#include "Expens.h"
#include <ctime>

Expense::Expense(const std::string& category, double amount, const std::string& description)
    : _category(category), _amount(amount), _description(description) {
    _date = std::time(nullptr); // Текущая дата и время
}

std::string Expense::getCategory() const {
    return _category;
}

double Expense::getAmount() const {
    return _amount;
}

std::string Expense::getDescription() const {
    return _description;
}

std::time_t Expense::getDate() const {
    return _date;
}