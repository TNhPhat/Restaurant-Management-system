#include "FileRestaurantRepository.hpp"

FileRestaurantRepository::FileRestaurantRepository(std::string filePath,
                                                   const IMenuRepository &menuRepository) {
    this->m_FileHandle = std::make_unique<JsonHandle>();
    this->m_FileHandle->LoadFile(filePath);
    json data = this->m_FileHandle->GetDaTa();

    const auto ResPtr = std::make_unique<Restaurant>(data["Restaurant Name"].get<std::string>());
    this->IRestaurantRepository::SaveRestaurant(std::move(ResPtr));
    for (const auto &branchData: data["Branches"]) {
        int branchID = branchData["Branch ID"].get<int>();
        auto branchName = branchData["Branch Name"].get<std::string>();
        auto branchLocation = branchData["Address"].get<std::string>();

        auto menu = menuRepository.GetMenuByID(branchData["Menu ID"].get<int>());
        const auto BranchPtr = std::make_shared<Branch>(branchID, branchName, branchLocation, menu);
        this->IRestaurantRepository::SaveBranch(BranchPtr);
    }
    LOG_INFO("Restaurant data loaded successfully from file: {}", filePath);
}

void FileRestaurantRepository::SaveRestaurantData(std::string filePath) const {
    json restaurantData;
    restaurantData["Restaurant Name"] = this->IRestaurantRepository::GetRestaurant().GetName();
    restaurantData["Branches"] = json::array();
    for (const auto &branch: this->IRestaurantRepository::GetBranches()) {
        json branchData;
        branchData["Branch ID"] = branch->GetID();
        branchData["Branch Name"] = branch->GetName();
        branchData["Address"] = branch->GetLocation();
        branchData["Menu ID"] = branch->GetMenu()->GetID();
        restaurantData["Branches"].push_back(branchData);
    }
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        LOG_ERROR("Failed to open file {} for writing.", filePath);
        return;
    }
    outFile << restaurantData.dump(4);
    outFile.close();
    LOG_INFO("Restaurant data saved successfully to {}", filePath);
}
