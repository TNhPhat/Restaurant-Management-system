#include <iostream>
#include "Filehandle.hpp"
#include "IDManager.hpp"

int main() {
    JsonHandle* JsonHandler = new JsonHandle();
    IDManager IDManager("Data/IDRegistry.json", JsonHandler);
    IDManager::SetInstance(&IDManager);
    std::cout <<  IDManager::GetInstance().GetNextID("Bill");
    std::cout << IDManager.GetNextID("Bill");
    return 0;
}