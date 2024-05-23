#pragma once
#include "FinancialManager.h"

class Menu {
public:
    static void showMenu();
    static void run(FinanceManager& manager);
    static bool askContinue();
};

