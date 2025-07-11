#include <iostream>
#include "OrderTest.hpp"
#include "IDManager.hpp"

int main()
{
    IDManager::Init("Data/IDRegistry.json");

    //BudgetTest();
    OrderTest();
    return 0;
}