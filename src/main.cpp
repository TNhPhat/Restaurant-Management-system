#include <iostream>
#include "Infrastructure/FileHandle/FileHandle.hpp"

int main() {
    JsonHandle j;
    j.LoadFile("test.json");

    // Show initial data
    std::cout << "Loaded JSON data: " << j.GetDaTa().dump(4) << std::endl;

    // Set a nested key
    json newValue = {2};
    JsonCommand* setCmd = new SetKeyCommand({"name", "first"}, newValue);
    j.ExecuteCommand(setCmd);
    std::cout << "After SetKeyCommand: " << j.GetDaTa().dump(4) << std::endl;

    // Undo set
    j.Undo();
    std::cout << "After Undo: " << j.GetDaTa().dump(4) << std::endl;

    // Redo set
    j.Redo();
    std::cout << "After Redo: " << j.GetDaTa().dump(4) << std::endl;

    // Get a key's data
    json keyData = j.GetKeyData({"menuItem"});
    std::cout << "Key 'menuItem' data: " << keyData.dump(4) << std::endl;

    // Add a new value at root
    json newValue2 = {{"New Value", "Another Value"}};
    JsonCommand* addCmd = new AddDataCommand({"name"}, newValue2);
    j.ExecuteCommand(addCmd);
    std::cout << "After AddDataCommand: " << j.GetDaTa().dump(4) << std::endl;

    // Undo add
    j.Undo();
    std::cout << "After Undo Add: " << j.GetDaTa().dump(4) << std::endl;

    // Redo add
    j.Redo();
    std::cout << "After Redo Add: " << j.GetDaTa().dump(4) << std::endl;

    JsonCommand* remov = new RemoveDataCommand({"name"});
    j.ExecuteCommand(remov);
    std::cout << "After RemoveDataCommand: " << j.GetDaTa().dump(4) << std::endl;

    // Undo remove
    j.Undo();
    std::cout << "After Undo Remove: " << j.GetDaTa().dump(4) << std::endl;

    // Redo remove
    j.Redo();
    std::cout << "After Redo Remove: " << j.GetDaTa().dump(4) << std::endl;

    return 0;
}