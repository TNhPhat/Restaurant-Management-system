#include "UI/UICore/Core.hpp"

int main() {
    Core core;
    core.Init();
    core.Start();
    core.Shutdown();
    return 0;
}
