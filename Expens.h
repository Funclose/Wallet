#pragma once
#include <string>
#include <ctime>

class Expense {
private:
    std::string _category;
    double _amount;
    std::string _description;
    std::time_t _date;

public:
    Expense(const std::string& category, double amount, const std::string& description = "");

    std::string getCategory() const;
    double getAmount() const;
    std::string getDescription() const;
    std::time_t getDate() const;
};

