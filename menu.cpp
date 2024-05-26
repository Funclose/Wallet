#include "menu.h"
#include <iostream>
#include <memory>
#include <string>
#include "FinancialManager.h"
#include "wallet.h"
void Menu::showMenu() {
    std::cout << "\nСистема управления персональными финансами\n";
    std::cout << "1. Добавить кошелек/карту\n";
    std::cout << "2. Пополнить кошелек/карту\n";
    std::cout << "3. Добавить расход\n";
    std::cout << "4. Показать балансы\n";
    std::cout << "5. Показать расходы\n";
    std::cout << "6. Сформировать отчет\n";
    std::cout << "7. Сформировать ТОП-3 расходов\n";
    std::cout << "8. Сформировать ТОП-3 категорий\n";
    std::cout << "9. Сохранить отчеты в файл\n";
    std::cout << "0. Выйти\n";
    std::cout << "Выберите опцию: ";
}

bool Menu::askContinue() {
    char choice;
    while (true) {
        std::cout << "\nХотите продолжить? (y/n): ";
        std::cin >> choice;
        std::cin.ignore(); 
        if (choice == 'y' || choice == 'Y') {
            return true;
        }
        else if (choice == 'n' || choice == 'N') {
            return false;
        }
        else {
            std::cout << "Неверный выбор. Пожалуйста, введите 'y' или 'n'.\n";
        }
    }
}

void Menu::run(FinanceManager& manager) {
    while (true) {
        showMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(); 

        if (choice == 0) {
            break;
        }

        switch (choice) {
        case 1: {
            std::string name, type;
            double balance;
            std::cout << "Введите имя кошелька/карты: ";
            std::getline(std::cin, name);
            std::cout << "Введите тип (1.debit/2.credit): ";
            std::getline(std::cin, type);
            if (type == "1") {
                type = "debit";
            }
            else if (type == "2") {
                type = "credit";
            }
            else {
                std::cout << "Неверный тип. Установлен тип 'debit' по умолчанию.\n";
                type = "debit";
            }
            std::cout << "Введите начальный баланс: ";
            std::cin >> balance;
            std::cin.ignore();
            auto wallet = std::make_shared<Wallet>(name, type, balance);
            manager.addWallet(wallet);
            std::cout << "Кошелек/карта добавлен(а).\n";
            break;
        }
        case 2: {
            auto wallets = manager.getWallets();
            if (wallets.empty()) {
                std::cout << "Нет доступных кошельков для пополнения.\n";
                break;
            }

            std::cout << "Список доступных кошельков:\n";
            for (const auto& wallet : wallets) {
                std::cout << "- " << wallet->getName() << " (" << wallet->getType() << "), Баланс: " << wallet->getBalance() << "\n";
            }
            std::string name;
            double amount;
            std::cout << "Введите имя кошелька/карты для пополнения: ";
            std::getline(std::cin, name);
            std::cout << "Введите сумму пополнения: ";
            std::cin >> amount;
            std::cin.ignore();
            manager.depositToWallet(name, amount);
            break;
        }
        case 3: {
            auto wallets = manager.getWallets();
            if (wallets.empty()) {
                std::cout << "Нет доступных кошельков для добавления расхода.\n";
                break;
            }

            std::cout << "Список доступных кошельков:\n";
            for (const auto& wallet : wallets) {
                std::cout << "- " << wallet->getName() << " (" << wallet->getType() << "), Баланс: " << wallet->getBalance() << "\n";
            }
            std::string name, category, description;
            double amount;
            std::cout << "Введите имя кошелька/карты для расхода: ";
            std::getline(std::cin, name);
            std::cout << "Введите категорию расхода: ";
            std::getline(std::cin, category);
            std::cout << "Введите сумму расхода: ";
            std::cin >> amount;
            std::cin.ignore();
            std::cout << "Введите описание расхода: ";
            std::getline(std::cin, description);
            manager.addExpense(name, Expense(category, amount, description));
            break;
        }
        case 4:
            manager.showBalances();
            break;
        case 5:
            manager.showExpenses();
            break;
        case 6: {
            std::string period;
            std::cout << "Введите период отчета (day/week/month): ";
            std::getline(std::cin, period);
            manager.generateReport(period);
            break;
        }
        case 7: {
            std::string period;
            std::cout << "Введите период для ТОП-3 расходов (week/month): ";
            std::getline(std::cin, period);
            manager.generateTopExpenses(period, 3);
            break;
        }
        case 8: {
            std::string period;
            std::cout << "Введите период для ТОП-3 категорий (week/month): ";
            std::getline(std::cin, period);
            manager.generateTopCategories(period, 3);
            break;
        }
        case 9: {
            std::string filename;
            std::cout << "Введите имя файла для сохранения отчетов пример(text.txt): ";
            std::getline(std::cin, filename);
            manager.saveReportsToFile(filename);
            break;
        }
        default:
            std::cout << "Неверный выбор. Пожалуйста, попробуйте снова.\n";
        }

        if (!Menu::askContinue()) {
            break;
        }
    }
}
