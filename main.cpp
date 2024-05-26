#include "FinancialManager.h"
#include "menu.h"
#include <iostream>
#include <Windows.h>

int main() 
{
    setlocale(LC_ALL, "");
    FinanceManager manager;
    Menu::run(manager);
    return 0;
}