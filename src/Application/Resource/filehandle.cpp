#include"filehandle.hpp"
FileHandle::FileHandle():m_FilePath("") {}   

void JsonHandle::LoadFile(const std::string &FilePath) {
    //open
    this->m_FilePath = FilePath;
    std::fstream FileInputStream ;
    FileInputStream.open(FilePath,std::ios::in);
    if (!FileInputStream.is_open())
        LOG_ERROR("Can't open file: {}", FilePath);
    //Load Data
    try{
        FileInputStream >> this->m_Data;
    }
    catch (const json::parse_error &e) {
        LOG_ERROR("JSON parse error: {}", e.what());
    }
    //close
    FileInputStream.close();
}


SetKeyCommand::SetKeyCommand(std::vector<std::string> Key, json NewValue): m_Key(Key), m_NewValue(NewValue), m_OldValue(nullptr) {}
bool SetKeyCommand::execute(json &Data) {
    if (this->m_Key.empty()) {
            LOG_ERROR("Key vector is empty. Cannot set value.");
            return false;
    }
    json* current = &Data;
    for (size_t i = 0; i + 1 < m_Key.size(); ++i) {
        if (current->contains(this->m_Key[i]) && (*current)[this->m_Key[i]].is_object()) {
            current = &((*current)[this->m_Key[i]]);
        } else {
            LOG_WARNING("Path does not exist at '{}'. No value set.", m_Key[i]);
            return false;
        }
    }
    std::string lastKey = this->m_Key.back();
    if (current->contains(lastKey)) {
        this->m_OldValue = (*current)[lastKey];
        (*current)[lastKey] = this->m_NewValue;
        LOG_INFO("SetKeyCommand executed: Key = {}, New Value = {}", lastKey, m_NewValue.dump());
        return true;
    } 
    else {
        LOG_WARNING("Key '{}' does not exist. No value set.", lastKey);
        return false;
    }
}

bool SetKeyCommand::undo(json &Data) {
    if (this->m_Key.empty()) {
        LOG_ERROR("Key vector is empty. Cannot undo.");
        return false;
    }
    json* current = &Data;
    for (size_t i = 0; i + 1 < this->m_Key.size(); ++i) {
        if (current->contains(m_Key[i]) && (*current)[m_Key[i]].is_object()) {
            current = &((*current)[m_Key[i]]);
        } 
        else {
            LOG_WARNING("Path does not exist at '{}'. Cannot undo.", m_Key[i]);
            return false;
        }
    }
    std::string lastKey = m_Key.back();
    if (current->contains(lastKey)) {
        (*current)[lastKey] = m_OldValue;
        LOG_INFO("Undo SetKeyCommand: Key = {}, Old Value = {}", lastKey, m_OldValue.dump());
        return true;
    } 
    else {
        LOG_WARNING("Key '{}' does not exist. Cannot undo.", lastKey);
        return false;
    }
}

SetKeyCommand::~SetKeyCommand() {
    while (!this->m_Key.empty()) {
        this->m_Key.pop_back();
    }
}

AddDataCommand::AddDataCommand(std::vector<std::string> Key, json NewData): m_Key(Key), m_NewData(NewData), m_OldData(nullptr) {}

bool AddDataCommand::execute(json &Data) {
    json* current = &Data;
    if (this->m_Key.empty()) {
        this->m_OldData = *current;
        current->update(this->m_NewData);
        LOG_INFO("AddDataCommand executed: New Value = {}", m_NewData.dump());
        return true;
    }
    for (size_t i = 0; i + 1 < m_Key.size(); ++i) {
        if (current->contains(this->m_Key[i]) && (*current)[this->m_Key[i]].is_object()) {
            current = &((*current)[this->m_Key[i]]);
        } 
        else {
            LOG_WARNING("Path does not exist at '{}'. No value added.", m_Key[i]);
            return false;
        }
    }
    std::string lastKey = this->m_Key.back();
    if (current->contains(lastKey)) {
        this->m_OldData = (*current)[lastKey];
        (*current)[lastKey].update(this->m_NewData);
        LOG_INFO("AddDataCommand executed: Key = {}, New Value = {}", lastKey, m_NewData.dump());
        return true;
    } 
    else {
        LOG_WARNING("Key '{}' doesn't exists. No value added.", lastKey);
        return false;
    }
}

bool AddDataCommand::undo(json &Data) {
    json* current = &Data;
    if (this->m_Key.empty()) {
        *current = this->m_OldData; 
        LOG_INFO("Undo AddDataCommand: Entire data restored to old value.");
        return true;
    }
    else{
        for (size_t i = 0; i + 1 < this->m_Key.size(); ++i) {
            if (current->contains(m_Key[i]) && (*current)[m_Key[i]].is_object()) {
                current = &((*current)[m_Key[i]]);
            } 
            else {
                LOG_WARNING("Path does not exist at '{}'. Cannot undo add.", m_Key[i]);
                return false;
            }
        }
        std::string lastKey = m_Key.back();
        (*current)[lastKey] = this->m_OldData;
        LOG_INFO("Undo AddDataCommand: Key '{}' restored to old value.", lastKey);
        return true;
    }
    
}

AddDataCommand::~AddDataCommand() {
    while (!this->m_Key.empty()) {
        this->m_Key.pop_back();
    }
}

RemoveDataCommand::RemoveDataCommand(std::vector<std::string> Key): m_Key(Key), m_OldData(nullptr) {}
bool RemoveDataCommand::execute(json &Data) {
    if (this->m_Key.empty()) {
        LOG_ERROR("Key vector is empty. Cannot remove value.");
        return false;
    }
    json* current = &Data;
    for (size_t i = 0; i + 1 < m_Key.size(); ++i) {
        if (current->contains(this->m_Key[i]) && (*current)[this->m_Key[i]].is_object()) {
            current = &((*current)[this->m_Key[i]]);
        } 
        else {
            LOG_WARNING("Path does not exist at '{}'. No value removed.", m_Key[i]);
            return false;
        }
    }
    std::string lastKey = this->m_Key.back();
    if (current->contains(lastKey)) {
        this->m_OldData = (*current)[lastKey];
        current->erase(lastKey);
        LOG_INFO("RemoveDataCommand executed: Key = {}", lastKey);
        return true;
    } 
    else {
        LOG_WARNING("Key '{}' does not exist. No value removed.", lastKey);
        return false;
    }
}

bool RemoveDataCommand::undo(json &Data) {
    if (this->m_Key.empty()) {
        LOG_ERROR("Key vector is empty. Cannot undo remove.");
        return false;
    }
    json* current = &Data;
    for (size_t i = 0; i + 1 < this->m_Key.size(); ++i) {
        if (current->contains(m_Key[i]) && (*current)[m_Key[i]].is_object()) {
            current = &((*current)[m_Key[i]]);
        } 
        else {
            LOG_WARNING("Path does not exist at '{}'. Cannot undo remove.", m_Key[i]);
            return false;
        }
    }
    std::string lastKey = m_Key.back();
    if (m_OldData.is_null()) {
        LOG_WARNING("No old data to restore for key '{}'.", lastKey);
        return false;
    } 
    else {
        (*current)[lastKey] = this->m_OldData;
        LOG_INFO("Undo RemoveDataCommand: Key '{}' restored with old data.", lastKey);
        return true;
    }
}

RemoveDataCommand::~RemoveDataCommand() {
    while (!this->m_Key.empty()) {
        this->m_Key.pop_back();
    }
}

void JsonHandle::ExecuteCommand(JsonCommand* command){
    if (command){
        if(command->execute(this->m_Data)){
            this->m_UndoStack.push(command);      
            while (!this->m_RedoStack.empty()) {
                delete this->m_RedoStack.top();
                this->m_RedoStack.pop();
            }
        }
    }
}

void JsonHandle::Undo() {
    if (!this->m_UndoStack.empty()) {
        JsonCommand* command = this->m_UndoStack.top();
        this->m_UndoStack.pop();
        command->undo(this->m_Data);
        this->m_RedoStack.push(command);
    } 
    else {
        LOG_WARNING("Undo stack is empty.");
    }
}
void JsonHandle::Redo() {
    if (!this->m_RedoStack.empty()) {
        JsonCommand* command = this->m_RedoStack.top();
        this->m_RedoStack.pop();
        command->execute(this->m_Data);
        this->m_UndoStack.push(command);
    } 
    else {
        LOG_WARNING("Redo stack is empty.");
    }
}

void JsonHandle::SaveFile(){
    if (this->m_FilePath.empty()) {
        LOG_ERROR("File path is empty. Please specify a file path before saving.");
    }
    std::ofstream FileOutputStream(this->m_FilePath);
    if (!FileOutputStream.is_open()) {
        LOG_ERROR("Can't open file for writing: {}", this->m_FilePath);
        return;
    }
    try {
        FileOutputStream << m_Data.dump(4); 
    } 
    catch (const json::exception& e) {
        LOG_ERROR("JSON write error: {}", e.what());
    }
    FileOutputStream.close();
}

void JsonHandle::SaveFileAs(const std::string &SavePath) {
    this->m_FilePath = SavePath;
    this->SaveFile();
}

json JsonHandle::GetDaTa(){
    return this->m_Data;
}


JsonHandle::JsonHandle() {
    this->m_FilePath = "";
}

json JsonHandle::GetKeyData(const std::vector<std::string> &Key) {
    json* current = &this->m_Data;
    for (const auto& key : Key) {
        if (current->contains(key)) {
            current = &((*current)[key]);
        } 
        else{
            LOG_WARNING("Key '{}' not found in JSON data.", key);
            return json::object(); 
        }
    }
    return *current; 
}
JsonHandle::~JsonHandle() {
    while (!this->m_UndoStack.empty()) {
        delete this->m_UndoStack.top();
        this->m_UndoStack.pop();
    }
    while (!this->m_RedoStack.empty()) {
        delete this->m_RedoStack.top();
        this->m_RedoStack.pop();
    }
}