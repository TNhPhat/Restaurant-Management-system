#include <iostream>
#include "Filehandle.hpp"
#include "IDManager.hpp"

int main() {
    std::cout << IDManager.GetNextID("Bill");
    return 0;
}