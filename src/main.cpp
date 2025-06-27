#include <iostream>
#include "Filehandle.hpp"
#include "IDManager.hpp"

int main() {
    JsonHandle* JsonHandler = new JsonHandle();
    IDManager::Init("Data/IDRegistry.json", JsonHandler);
    std::cout <<  IDManager::GetInstance().GetNextID("Bill");
    
    return 0;
}