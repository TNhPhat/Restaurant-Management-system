#include <iostream>
#include "Filehandle.hpp"
#include "IDManager.hpp"

int main() {
    JsonHandle* JsonHandler = new JsonHandle();
    IDManager IDManager("Data/IDRegistry.json", JsonHandler);

    std::cout << IDManager.GetNextID("Bill");
   

    return 0;
}