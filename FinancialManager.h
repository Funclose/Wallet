#pragma once
#include "Wallet.h"
#include "Expens.h"
#include <vector>
#include <memory>
#include <string>
#include <map>

class FinanceManager {
private:
    std::vector<std::shared_ptr<Wallet>> _wallets;
    std::vector<Expense> _expenses;

    std::shared_ptr<Wallet> findWallet(const std::string& walletName) const;

    std::vector<Expense> getExpensesForPeriod(std::time_t start, std::time_t end) const;

    std::vector<std::shared_ptr<Wallet>> wallets;//
public:
    void addWallet(const std::shared_ptr<Wallet>& wallet);
    void addExpense(const std::string& walletName, const Expense& expense);
    void depositToWallet(const std::string& walletName, double amount);
    void showBalances() const;
    void showExpenses() const;
    std::vector<std::shared_ptr<Wallet>> getWallets() const;//

    // Отчеты и рейтинги
    void generateReport(const std::string& period) const;
    void generateTopExpenses(const std::string& period, int topN) const;
    void generateTopCategories(const std::string& period, int topN) const;

    // Сохранение отчетов и рейтингов в файл
    void saveReportsToFile(const std::string& filename) const;
};

