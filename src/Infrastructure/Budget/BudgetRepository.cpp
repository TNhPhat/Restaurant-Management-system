#include "BudgetRepository.hpp"
#include "Logger.hpp"
#include "FileHandle.hpp"
#include "DateTime.hpp"

BudgetRepository::BudgetRepository(const std::string& FilePath, JsonHandle* FileHandler)
    : m_FileHandler(FileHandler), m_FilePath(FilePath)
{
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

    if (!m_FileHandler->GetDaTa().contains("bills")) {
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

    if (!Data.contains("bills")) return Result;

    for (const auto& BillJson : Data["bills"]) {
        int ID = BillJson.value("id", 0);
        std::string Message = BillJson.value("message", "");
        double Total = BillJson.value("total", 0.0);
        std::string DateStr = BillJson.value("date", "");
        std::string TypeStr = BillJson.value("type", "income");

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

    std::vector<std::string> Path = { "bills" };
    m_FileHandler->ExecuteCommand(new SetKeyCommand(Path, NewBillsArray));
    
    m_FileHandler->SaveFile();
}

bool BudgetRepository::RemoveBillByID(const int &BillID) {
    json Data = m_FileHandler->GetDaTa();

    if (!Data.contains("Bill")) return false;

    auto& Bills = Data["Bill"];
    for (size_t i = 0; i < Bills.size(); ++i) {
        if (Bills[i].contains("ID") && Bills[i]["ID"] == BillID) {
            std::vector<std::string> Path = { "Bill" };
            m_FileHandler->ExecuteCommand(new RemoveDataCommand(Path, i));
            m_FileHandler->SaveFile();
            return true;
        }
    }

    return false;
}