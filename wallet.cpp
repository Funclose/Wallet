#include "wallet.h"
#include <iostream>

Wallet::Wallet(const std::string& name, const std::string& type, double initialBalance)
    : _name(name), _balance(initialBalance), _type(type) {}

std::string Wallet::getName() const {
    return _name;
}

std::string Wallet::getType() const {
    return _type;
}

double Wallet::getBalance() const {
    return _balance;
}

void Wallet::deposit(double amount) {
    _balance += amount;
    std::cout << "������ " << _name << " �������� �� " << amount << ". ����� ������: " << _balance << std::endl;
}

bool Wallet::withdraw(double amount) {
    if (_balance >= amount) {
        _balance -= amount;
        std::cout << "����� " << amount << " � " << _name << ". ����� ������: " << _balance << std::endl;
        return true;
    }
    else {
        std::cout << "������������ ������� �� " << _name << " ��� ������ " << amount << std::endl;
        return false;
    }
}