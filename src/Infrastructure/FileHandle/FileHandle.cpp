#include"FileHandle.hpp"

FileHandle::FileHandle():m_FilePath("") {}   
std::string FileHandle::GetFilePath() const{
    return this->m_FilePath;
}
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

PushArrayCommand::PushArrayCommand(const std::vector<std::string>& Key, const json& NewElement)
    : m_Key(Key), m_NewElement(NewElement) {}

PushArrayCommand::~PushArrayCommand() {
    while (!this->m_Key.empty()) {
        this->m_Key.pop_back();
    }
}

bool PushArrayCommand::execute(json& Data) {
    json* current = &Data;

    for (size_t i = 0; i + 1 < m_Key.size(); ++i) {
        const std::string& key = m_Key[i];

        if (current->contains(key)) {
            current = &(*current)[key];

            if (!current->is_object()) {
                LOG_ERROR("Expected object at '{}', but found non-object.", key);
                return false;
            }
        } else {
            LOG_WARNING("PushArrayCommand failed: Key '{}' not found.", key);
            return false;
        }
    }

    std::string lastKey = m_Key.back();

    if (!current->contains(lastKey)) {
        (*current)[lastKey] = json::array();
    }

    if (!(*current)[lastKey].is_array()) {
        LOG_ERROR("Target for PushArrayCommand is not an array: '{}'", lastKey);
        return false;
    }

    (*current)[lastKey].push_back(m_NewElement);
    LOG_INFO("PushArrayCommand executed: New element = {}", m_NewElement.dump());
    return true;
}

bool PushArrayCommand::undo(json& Data) {
    json* current = &Data;

    for (const auto& key : m_Key) {
        if (current->contains(key)) {
            current = &(*current)[key];
        } else {
            LOG_WARNING("PushArrayCommand undo failed: Key '{}' not found.", key);
            return false;
        }
    }

    if (!current->is_array() || current->empty()) {
        LOG_ERROR("Undo failed: target is not a non-empty array.");
        return false;
    }

    current->erase(current->end() - 1);  
    LOG_INFO("PushArrayCommand undo executed: Removed last element.");
    return true;
}

RemoveDataCommand::RemoveDataCommand(std::vector<std::string> Key)
    : m_Key(std::move(Key)), m_OldData(nullptr), m_ArrayIndex(std::nullopt) {}

RemoveDataCommand::RemoveDataCommand(std::vector<std::string> Key, size_t Index)
    : m_Key(std::move(Key)), m_OldData(nullptr), m_ArrayIndex(Index) {}

bool RemoveDataCommand::execute(json &Data) {
    if (m_Key.empty()) {
        LOG_ERROR("Key vector is empty. Cannot remove.");
        return false;
    }

    json* current = &Data;
    for (const auto& key : m_Key) {
        if (!current->contains(key)) {
            LOG_WARNING("RemoveDataCommand: Key '{}' not found.", key);
            return false;
        }
        current = &(*current)[key];
    }

    if (m_ArrayIndex.has_value()) {
        if (!current->is_array() || m_ArrayIndex.value() >= current->size()) {
            LOG_ERROR("RemoveDataCommand: Invalid array index {}", m_ArrayIndex.value());
            return false;
        }
        m_OldData = (*current)[m_ArrayIndex.value()];
        current->erase(current->begin() + m_ArrayIndex.value());
        LOG_INFO("RemoveDataCommand: Removed array element at index {}", m_ArrayIndex.value());
        return true;
    } else {
        std::string lastKey = m_Key.back();
        if (current->contains(lastKey)) {
            m_OldData = (*current)[lastKey];
            current->erase(lastKey);
            LOG_INFO("RemoveDataCommand: Removed object key '{}'", lastKey);
            return true;
        }
        return false;
    }
}

bool RemoveDataCommand::undo(json &Data) {
    json* current = &Data;
    for (const auto& key : m_Key) {
        if (!current->contains(key)) {
            LOG_WARNING("RemoveDataCommand undo: Key '{}' not found.", key);
            return false;
        }
        current = &(*current)[key];
    }

    if (m_ArrayIndex.has_value()) {
        if (!current->is_array() || m_ArrayIndex.value() > current->size()) {
            LOG_ERROR("RemoveDataCommand undo: Invalid index.");
            return false;
        }
        current->insert(current->begin() + m_ArrayIndex.value(), m_OldData);
        LOG_INFO("RemoveDataCommand undo: Restored array element at index {}", m_ArrayIndex.value());
        return true;
    } else {
        std::string lastKey = m_Key.back();
        (*current)[lastKey] = m_OldData;
        LOG_INFO("RemoveDataCommand undo: Restored object key '{}'", lastKey);
        return true;
    }
}

RemoveDataCommand::~RemoveDataCommand() {
    while (!this->m_Key.empty()) {
        this->m_Key.pop_back();
    }
}

void JsonHandle::ExecuteCommand(JsonCommand* command){
    if (command)
        if(command->execute(this->m_Data)){
            this->m_UndoDeque.push_back(command);
            if(this->m_UndoDeque.size() > 15){
                delete this->m_UndoDeque.front();
                this->m_UndoDeque.pop_front();
            }
            while(!this->m_RedoDeque.empty()){
                delete this->m_RedoDeque.back();
                this->m_RedoDeque.pop_back();
            }
        }
}

void JsonHandle::Undo() {
    if (!this->m_UndoDeque.empty()) {
        JsonCommand* command = this->m_UndoDeque.back();
        this->m_UndoDeque.pop_back();
        command->undo(this->m_Data);
        this->m_RedoDeque.push_front(command);
        if(this->m_RedoDeque.size() > 30){
            delete this->m_RedoDeque.back();
            this->m_UndoDeque.pop_back();
        }
    } 
    else {
        LOG_WARNING("Undo deque is empty.");
    }
}
void JsonHandle::Redo() {
    if (!this->m_RedoDeque.empty()) {
        JsonCommand* command = this->m_RedoDeque.front();
        this->m_RedoDeque.pop_front();
        command->execute(this->m_Data);
        this->m_UndoDeque.push_back(command);
        if(this->m_UndoDeque.size() > 30){
            delete this->m_UndoDeque.front();
            this->m_UndoDeque.pop_front();
        }
    } 
    else {
        LOG_WARNING("Redo deque is empty.");
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
    while (!this->m_UndoDeque.empty()) {
        delete this->m_UndoDeque.back();
        this->m_UndoDeque.pop_back();
    }
    while (!this->m_RedoDeque.empty()) {
        delete this->m_RedoDeque.front();
        this->m_RedoDeque.pop_front();
    }
}