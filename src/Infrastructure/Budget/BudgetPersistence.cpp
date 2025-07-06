#include "BudgetPersistence.hpp"
#include "Logger.hpp"
#include "FileHandle.hpp"
#include "DateTime.hpp"

BudgetPersistence::BudgetPersistence(const std::string& FilePath, JsonHandle* FileHandler)
    : m_FileHandler(FileHandler), m_FilePath(FilePath)
{
    m_FileHandler->LoadFile(FilePath);
}

void BudgetPersistence::SaveBill(const std::shared_ptr<Bill>& Bill)
{
    json BillData = {
        {"id", Bill->GetID()},
        {"date", Bill->GetDate().ToStringDate()},
        {"message", Bill->GetMessage()},
        {"total", Bill->GetTotal()},
        {"type", BillTypeToString(Bill->GetType())}
    };

    if (!m_FileHandler->GetDaTa().contains("bills")) {
        m_FileHandler->ExecuteCommand(new SetKeyCommand({ "bills" }, json::array()));
    }

    std::vector<std::string> Path = { "bills" };
    m_FileHandler->ExecuteCommand(new PushArrayCommand(Path, BillData));
    m_FileHandler->SaveFile();
}

std::vector<std::shared_ptr<Bill>> BudgetPersistence::LoadAllBills()
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
        std::string TypeStr = BillJson.value("type", "Income");

        DateTime Date = DateTime::FromDateTimeString(DateStr);
        BillType Type = StringToBillType(TypeStr);

        Result.push_back(std::make_shared<Bill>(ID, Date, Message, Total, Type));
    }

    return Result;
}

bool BudgetPersistence::RemoveBillByID(const int &BillID)
{
    json Data = m_FileHandler->GetDaTa();

    if (!Data.contains("bills")) return false;

    auto& Bills = Data["bills"];
    for (size_t i = 0; i < Bills.size(); ++i) {
        if (Bills[i].contains("id") && Bills[i]["id"] == BillID) {
            std::vector<std::string> Path = { "bills" };
            m_FileHandler->ExecuteCommand(new RemoveDataCommand(Path, i));
            m_FileHandler->SaveFile();
            return true;
        }
    }

    return false;
}