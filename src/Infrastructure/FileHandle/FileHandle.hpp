#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include"json.hpp"
#include"../Logging/Logger.hpp"
#include<deque>
#include <vector>
using json = nlohmann::json;
class FileHandle{
public:
    FileHandle();
    ~FileHandle() = default;
    virtual void LoadFile(const std::string &FilePath) = 0;
    virtual void SaveFile() = 0;
    virtual void SaveFileAs(const std::string &SavePath) = 0;
    // virtual void CloseFile() = 0;
protected:
    std::string m_FilePath;
};

class JsonCommand{
    public:
        virtual bool execute(json &Data) = 0;                   
        virtual bool undo(json &Data) = 0;
};

class SetKeyCommand: public JsonCommand{
public:
    SetKeyCommand(std::vector<std::string> m_Key,json NewValue);
    ~SetKeyCommand();
    bool execute(json &Data) override;
    bool undo(json &Data) override;

private:
    std::vector<std::string> m_Key;
    json m_NewValue;
    json m_OldValue;
};

class AddDataCommand: public JsonCommand{
public:
    AddDataCommand(std::vector<std::string> m_Key, json NewValue);
    ~AddDataCommand();
    bool execute(json &Data) override;
    bool undo(json &Data) override;
private:
    std::vector<std::string> m_Key;
    json m_NewData;
    json m_OldData;
};

class PushArrayCommand : public JsonCommand {
private:
    std::vector<std::string> m_Key;
    json m_NewElement;

public:
    PushArrayCommand(const std::vector<std::string>& Key, const json& NewElement);
    ~PushArrayCommand();

    bool execute(json& Data) override;
    bool undo(json& Data) override;
};

class RemoveDataCommand: public JsonCommand{
public: 
    RemoveDataCommand(std::vector<std::string> m_Key);
    RemoveDataCommand(std::vector<std::string> Key, size_t Index);
    ~RemoveDataCommand();
    bool execute(json &Data) override;
    bool undo(json &Data) override;
private:
    std::vector<std::string> m_Key;
    json m_OldData;
    std::optional<size_t> m_ArrayIndex;
};

class JsonHandle: public FileHandle{
public:
    void LoadFile(const std::string &FilePath) override;
    json GetDaTa();
    json GetKeyData(const std::vector<std::string> &Key);
    void ExecuteCommand(JsonCommand* command);
    void Undo();
    void SaveFile() override;
    void SaveFileAs(const std::string &SavePath) override;
    void Redo();
    ~JsonHandle();
    JsonHandle();
private:
    json m_Data;
    std::deque<JsonCommand*> m_UndoDeque;
    std::deque<JsonCommand*> m_RedoDeque;
    
};
