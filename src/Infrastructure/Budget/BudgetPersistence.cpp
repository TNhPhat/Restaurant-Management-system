#include "BudgetPersistence.hpp"
#include "Logger.hpp"
#include "FileHandle.hpp"
#include "DateTime.hpp"

BudgetPersistence::BudgetPersistence(const std::string& FilePath, JsonHandle* FileHandler)
    : m_FileHandler(FileHandler), m_FilePath(FilePath)
{
    m_FileHandler->LoadFile(FilePath);
}

void BudgetPersistence::SaveBill(const std::shared_ptr<Bill>& bill)
{
    json billData = {
        {"id", bill->GetID()},
        {"date", bill->GetDate().ToString()},
        {"message", bill->GetMessage()},
        {"total", bill->GetTotal()},
        {"type", BillTypeToString(bill->GetType())}
    };

    std::vector<std::string> path = { "bills" };
    m_FileHandler->ExecuteCommand(new AddDataCommand(path, billData));
    m_FileHandler->SaveFile();
}

std::vector<std::shared_ptr<Bill>> BudgetPersistence::LoadAllBills()
{
    m_FileHandler->LoadFile(m_FilePath);
    json data = m_FileHandler->GetDaTa();
    std::vector<std::shared_ptr<Bill>> result;

    if (!data.contains("bills")) return result;

    for (const auto& billJson : data["bills"]) {
        int id = billJson.value("id", 0);
        std::string message = billJson.value("message", "");
        double total = billJson.value("total", 0.0);
        std::string dateStr = billJson.value("date", "");
        std::string typeStr = billJson.value("type", "Income");

        DateTime date = DateTime::FromDateTimeString(dateStr);
        BillType type = StringToBillType(typeStr);

        result.push_back(std::make_shared<Bill>(id, date, message, total, type));
    }

    return result;
}

bool BudgetPersistence::RemoveBillByID(int billID)
{
    json data = m_FileHandler->GetDaTa();

    if (!data.contains("bills")) return false;

    auto& bills = data["bills"];
    for (size_t i = 0; i < bills.size(); ++i) {
        if (bills[i].contains("id") && bills[i]["id"] == billID) {
            std::vector<std::string> path = { "bills" };
            m_FileHandler->ExecuteCommand(new RemoveDataCommand(path, i));
            m_FileHandler->SaveFile();
            return true;
        }
    }

    return false;
}
