#include "FinancialManager.h"
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <sstream>
//����� �������� �� �����
std::shared_ptr<Wallet> FinanceManager::findWallet(const std::string& walletName) const
{   
    for (const auto& wallet : _wallets) {
        if (wallet->getName() == walletName) {
            return wallet;
        }
    }
    throw std::runtime_error("������� " + walletName + " �� ������");
}

//����� ������� �� ������� ������������
void FinanceManager::addWallet(const std::shared_ptr<Wallet>& wallet) {
    _wallets.push_back(wallet);
}

//��������� �������
void FinanceManager::addExpense(const std::string& walletName, const Expense& expense) {
    try {
        auto wallet = findWallet(walletName);
        if (wallet->withdraw(expense.getAmount())) {
            _expenses.push_back(expense);
            std::cout << "������ �������� � ���������: " << expense.getCategory() << ", �����: " << expense.getAmount() << std::endl;
        }
        else {
            std::cout << "������: ������������ ������� �� �������� " << walletName << std::endl;
        }
    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}

//���������� ������� ��������
void FinanceManager::depositToWallet(const std::string& walletName, double amount) {
    try {
        auto wallet = findWallet(walletName);
        wallet->deposit(amount);
    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}

//���������� ������ ���� ���������
void FinanceManager::showBalances() const 
{
    for (const auto& wallet : _wallets)
    {
        std::cout << "�������: " << wallet->getName() << ", ���: " << wallet->getType() << ", ������: " << wallet->getBalance() << std::endl;
    }
}

//���������� ��� �������
void FinanceManager::showExpenses() const 
{
    for (const auto& expense : _expenses) 
    {
        std::cout << "���������: " << expense.getCategory() << ", �����: " << expense.getAmount() << ", ��������: " << expense.getDescription() << std::endl;
    }
}

//�������� ������� �� ������������ ������
std::vector<Expense> FinanceManager::getExpensesForPeriod(std::time_t start, std::time_t end) const {
    std::vector<Expense> result;
    for (const auto& expense : _expenses) 
    {
        if (expense.getDate() >= start && expense.getDate() <= end)
        {
            result.push_back(expense);
        }
    }
    return result;
}

//���������� ����� �� ������
void FinanceManager::generateReport(const std::string& period) const 
{
    std::time_t now = std::time(nullptr);
    std::tm now_tm = {};
    localtime_s(&now_tm, &now);
    std::time_t start, end;

    if (period == "day")
    {
        now_tm.tm_hour = 0;
        now_tm.tm_min = 0;
        now_tm.tm_sec = 0;
        start = std::mktime(&now_tm);
        end = start + 24 * 60 * 60 - 1;
    }
    else if (period == "week") {
        now_tm.tm_hour = 0;
        now_tm.tm_min = 0;
        now_tm.tm_sec = 0;
        now_tm.tm_wday = 0;
        start = std::mktime(&now_tm);
        end = start + 7 * 24 * 60 * 60 - 1;
    }
    else if (period == "month") {
        now_tm.tm_hour = 0;
        now_tm.tm_min = 0;
        now_tm.tm_sec = 0;
        now_tm.tm_mday = 1;
        start = std::mktime(&now_tm);
        now_tm.tm_mon += 1;
        end = std::mktime(&now_tm) - 1;
    }
    else 
    {
        throw std::invalid_argument("�������� �������� ������");
    }

    auto expenses = getExpensesForPeriod(start, end);
    std::cout << "����� �� " << period << ":\n";
    for (const auto& expense : expenses) {
        std::cout << "���������: " << expense.getCategory() << ", �����: " << expense.getAmount() << ", ��������: " << expense.getDescription() << std::endl;
    }
}

//���������� ���-3 ������� �� �������� ������
void FinanceManager::generateTopExpenses(const std::string& period, int topN) const 
{
    std::time_t now = std::time(nullptr);
    std::tm now_tm = {};
    localtime_s(&now_tm, &now);
    std::time_t start, end;

    if (period == "week") {
        now_tm.tm_hour = 0;
        now_tm.tm_min = 0;
        now_tm.tm_sec = 0;
        now_tm.tm_wday = 0;
        start = std::mktime(&now_tm);
        end = start + 7 * 24 * 60 * 60 - 1;
    }
    else if (period == "month") {
        now_tm.tm_hour = 0;
        now_tm.tm_min = 0;
        now_tm.tm_sec = 0;
        now_tm.tm_mday = 1;
        start = std::mktime(&now_tm);
        now_tm.tm_mon += 1;
        end = std::mktime(&now_tm) - 1;
    }
    else 
    {
        throw std::invalid_argument("������ �������� ������");
    }

    auto expenses = getExpensesForPeriod(start, end);
    std::sort(expenses.begin(), expenses.end(), [](const Expense& a, const Expense& b) //��������� ������� (������ ���������)
        {         
        return a.getAmount() > b.getAmount();
        });

    std::cout << "���-" << topN << " �������� �� " << period << ":\n";
    for (int i = 0; i < std::min(topN, static_cast<int>(expenses.size())); ++i) 
    {
        std::cout << "���������: " << expenses[i].getCategory() << ", �����: " << expenses[i].getAmount() << ", ��������: " << expenses[i].getDescription() << std::endl;
    }
}

void FinanceManager::generateTopCategories(const std::string& period, int topN) const {
    std::time_t now = std::time(nullptr);
    std::tm now_tm = {};
    localtime_s(&now_tm, &now);
    std::time_t start, end;

    if (period == "week") {
        now_tm.tm_hour = 0;
        now_tm.tm_min = 0;
        now_tm.tm_sec = 0;
        now_tm.tm_wday = 0;
        start = std::mktime(&now_tm);
        end = start + 7 * 24 * 60 * 60 - 1;
    }
    else if (period == "month") 
    {
        now_tm.tm_hour = 0;
        now_tm.tm_min = 0;
        now_tm.tm_sec = 0;
        now_tm.tm_mday = 1;
        start = std::mktime(&now_tm);
        now_tm.tm_mon += 1;
        end = std::mktime(&now_tm) - 1;
    }
    else 
    {
        throw std::invalid_argument("������ �������� ������");
    }

    auto expenses = getExpensesForPeriod(start, end);
    std::map<std::string, double> categoryTotals;
    for (const auto& expense : expenses) {
        categoryTotals[expense.getCategory()] += expense.getAmount();
    }

    std::vector<std::pair<std::string, double>> categoryVector(categoryTotals.begin(), categoryTotals.end());
    std::sort(categoryVector.begin(), categoryVector.end(), [](const std::pair<std::string, double>& a, const std::pair<std::string, double>& b)
        {  
        return a.second > b.second;
        });

    std::cout << "���-" << topN << " ��������� �� " << period << ":\n";
    for (int i = 0; i < std::min(topN, static_cast<int>(categoryVector.size())); ++i) {
        std::cout << "���������: " << categoryVector[i].first << ", �����: " << categoryVector[i].second << std::endl;
    }
}

//�������� �������
void FinanceManager::saveReportsToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    file << "����� �� ����:\n";
    auto dayExpenses = getExpensesForPeriod(std::time(nullptr) - 24 * 60 * 60, std::time(nullptr));
    for (const auto& expense : dayExpenses) {
        file << "���������: " << expense.getCategory() << ", �����: " << expense.getAmount() << ", ��������: " << expense.getDescription() << std::endl;
    }

    file << "\n����� �� ������:\n";
    auto weekExpenses = getExpensesForPeriod(std::time(nullptr) - 7 * 24 * 60 * 60, std::time(nullptr));
    for (const auto& expense : weekExpenses) {
        file << "���������: " << expense.getCategory() << ", �����: " << expense.getAmount() << ", ��������: " << expense.getDescription() << std::endl;
    }

    file << "\n����� �� �����:\n";
    auto monthExpenses = getExpensesForPeriod(std::time(nullptr) - 30 * 24 * 60 * 60, std::time(nullptr));
    for (const auto& expense : monthExpenses) {
        file << "���������: " << expense.getCategory() << ", �����: " << expense.getAmount() << ", ��������: " << expense.getDescription() << std::endl;
    }

    file.close();
    std::cout << "������ ��������� � ����: " << filename << std::endl;
}