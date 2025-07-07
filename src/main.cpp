#include <iostream>
#include "BudgetTest.hpp"

int main()
{
    JsonHandle* IDManagerJson = new JsonHandle();
    IDManager::Init("Data/IDRegistry.json", IDManagerJson);

    BudgetTest();
    return 0;
}