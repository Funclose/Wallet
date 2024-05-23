#include "FinancialManager.h"
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <sstream>
//поиск кошелька по имени
std::shared_ptr<Wallet> FinanceManager::findWallet(const std::string& walletName) const
{   
    for (const auto& wallet : _wallets) {
        if (wallet->getName() == walletName) {
            return wallet;
        }
    }
    throw std::runtime_error("Кошелек " + walletName + " не найден");
}

//новый кошелек по желанию пользователя
void FinanceManager::addWallet(const std::shared_ptr<Wallet>& wallet) {
    _wallets.push_back(wallet);
}

//добавляет расходы
void FinanceManager::addExpense(const std::string& walletName, const Expense& expense) {
    try {
        auto wallet = findWallet(walletName);
        if (wallet->withdraw(expense.getAmount())) {
            _expenses.push_back(expense);
            std::cout << "Расход добавлен в категорию: " << expense.getCategory() << ", сумма: " << expense.getAmount() << std::endl;
        }
        else {
            std::cout << "Ошибка: недостаточно средств на кошельке " << walletName << std::endl;
        }
    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}

//пополнения баланса кошелька
void FinanceManager::depositToWallet(const std::string& walletName, double amount) {
    try {
        auto wallet = findWallet(walletName);
        wallet->deposit(amount);
    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}

//показывает баланс всех кошельков
void FinanceManager::showBalances() const 
{
    for (const auto& wallet : _wallets)
    {
        std::cout << "Кошелек: " << wallet->getName() << ", тип: " << wallet->getType() << ", баланс: " << wallet->getBalance() << std::endl;
    }
}

//показывает все расходы
void FinanceManager::showExpenses() const 
{
    for (const auto& expense : _expenses) 
    {
        std::cout << "Категория: " << expense.getCategory() << ", сумма: " << expense.getAmount() << ", описание: " << expense.getDescription() << std::endl;
    }
}

//получает расходы за определенный период
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

//генерирует отчет за период
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
        throw std::invalid_argument("Указаный неверный период");
    }

    auto expenses = getExpensesForPeriod(start, end);
    std::cout << "Отчет за " << period << ":\n";
    for (const auto& expense : expenses) {
        std::cout << "Категория: " << expense.getCategory() << ", сумма: " << expense.getAmount() << ", описание: " << expense.getDescription() << std::endl;
    }
}

//Генерирует Топ-3 расхода за указаный период
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
        throw std::invalid_argument("Указан неверный период");
    }

    auto expenses = getExpensesForPeriod(start, end);
    std::sort(expenses.begin(), expenses.end(), [](const Expense& a, const Expense& b) //анонимная функция (лямбда выражения)
        {         
        return a.getAmount() > b.getAmount();
        });

    std::cout << "ТОП-" << topN << " расходов за " << period << ":\n";
    for (int i = 0; i < std::min(topN, static_cast<int>(expenses.size())); ++i) 
    {
        std::cout << "Категория: " << expenses[i].getCategory() << ", сумма: " << expenses[i].getAmount() << ", описание: " << expenses[i].getDescription() << std::endl;
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
        throw std::invalid_argument("Указан неверный период");
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

    std::cout << "ТОП-" << topN << " категорий за " << period << ":\n";
    for (int i = 0; i < std::min(topN, static_cast<int>(categoryVector.size())); ++i) {
        std::cout << "Категория: " << categoryVector[i].first << ", сумма: " << categoryVector[i].second << std::endl;
    }
}

//Файловая система
void FinanceManager::saveReportsToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    file << "Отчет за день:\n";
    auto dayExpenses = getExpensesForPeriod(std::time(nullptr) - 24 * 60 * 60, std::time(nullptr));
    for (const auto& expense : dayExpenses) {
        file << "Категория: " << expense.getCategory() << ", сумма: " << expense.getAmount() << ", описание: " << expense.getDescription() << std::endl;
    }

    file << "\nОтчет за неделю:\n";
    auto weekExpenses = getExpensesForPeriod(std::time(nullptr) - 7 * 24 * 60 * 60, std::time(nullptr));
    for (const auto& expense : weekExpenses) {
        file << "Категория: " << expense.getCategory() << ", сумма: " << expense.getAmount() << ", описание: " << expense.getDescription() << std::endl;
    }

    file << "\nОтчет за месяц:\n";
    auto monthExpenses = getExpensesForPeriod(std::time(nullptr) - 30 * 24 * 60 * 60, std::time(nullptr));
    for (const auto& expense : monthExpenses) {
        file << "Категория: " << expense.getCategory() << ", сумма: " << expense.getAmount() << ", описание: " << expense.getDescription() << std::endl;
    }

    file.close();
    std::cout << "Отчеты сохранены в файл: " << filename << std::endl;
}