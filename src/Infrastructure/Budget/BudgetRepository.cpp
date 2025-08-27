#include <memory>
#include "BudgetRepository.hpp"
#include "Logger.hpp"
#include "FileHandle.hpp"
#include "DateTime.hpp"

BudgetRepository::BudgetRepository(const std::string& FilePath)
    : m_FilePath(FilePath)
{
    this->m_FileHandler = std::make_unique<JsonHandle>();
    m_FileHandler->LoadFile(FilePath);
}


void BudgetRepository::SaveBill(const std::shared_ptr<Bill>& Bill)
{
    json BillData = {
        {"ID", Bill->GetID()},
        {"Date", Bill->GetDate().ToStringDateTime()},
        {"Message", Bill->GetMessage()},
        {"Total", Bill->GetTotal()},
        {"Type", BillTypeToString(Bill->GetType())}
    };

    if (!m_FileHandler->GetDaTa().contains("Bills")) {
        m_FileHandler->ExecuteCommand(new SetKeyCommand({ "Bills" }, json::array()));
    }

    std::vector<std::string> Path = { "Bills" };
    m_FileHandler->ExecuteCommand(new PushArrayCommand(Path, BillData));
    m_FileHandler->SaveFile();
}

std::vector<std::shared_ptr<Bill>> BudgetRepository::LoadAllBills()
{
    m_FileHandler->LoadFile(m_FilePath);
    json Data = m_FileHandler->GetDaTa();
    std::vector<std::shared_ptr<Bill>> Result;

    if (!Data.contains("Bills")) return Result;

    for (const auto& BillJson : Data["Bills"]) {
        int ID = BillJson.value("ID", 0);
        std::string Message = BillJson.value("Message", "");
        double Total = BillJson.value("Total", 0.0);
        std::string DateStr = BillJson.value("Date", "");
        std::string TypeStr = BillJson.value("Type", "Income");

        DateTime Date = DateTime::FromDateTimeString(DateStr);
        BillType Type = StringToBillType(TypeStr);

        Result.push_back(std::make_shared<Bill>(ID, Date, Message, Total, Type));
    }

    return Result;
}

void BudgetRepository::SaveAllBills(const std::vector<std::shared_ptr<Bill>>& Bills) {
    json NewBillsArray = json::array();

    for (const auto& Bill : Bills) {
        json BillData = {
            {"ID", Bill->GetID()},
            {"Date", Bill->GetDate().ToStringDateTime()},
            {"Message", Bill->GetMessage()},
            {"Total", Bill->GetTotal()},
            {"Type", BillTypeToString(Bill->GetType())}
        };
        NewBillsArray.push_back(BillData);
    }

    std::vector<std::string> Path = { "Bills" };
    m_FileHandler->ExecuteCommand(new SetKeyCommand(Path, NewBillsArray));
    
    m_FileHandler->SaveFile();
}

bool BudgetRepository::RemoveBillByID(const int &BillID) {
    json Data = m_FileHandler->GetDaTa();

    if (!Data.contains("Bills")) return false;

    auto& Bills = Data["Bills"];
    for (size_t i = 0; i < Bills.size(); ++i) {
        if (Bills[i].contains("ID") && Bills[i]["ID"] == BillID) {
            std::vector<std::string> Path = { "Bills" };
            m_FileHandler->ExecuteCommand(new RemoveDataCommand(Path, i));
            m_FileHandler->SaveFile();
            return true;
        }
    }

    return false;
}