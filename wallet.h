#pragma once
#include <iostream>
class Wallet {
private:
    std::string _name;
    double _balance;
    std::string _type; // "debit" èëè "credit"

public:
    Wallet(const std::string& name, const std::string& type, double initialBalance = 0);

    std::string getName() const;
    std::string getType() const;
    double getBalance() const;

    void deposit(double amount);
    bool withdraw(double amount);
};

//hello
