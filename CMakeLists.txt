cmake_minimum_required(VERSION 3.16)

project(RestaurantManagementSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add ImGUI
include(FetchContent)

FetchContent_Declare(imgui_source
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)

FetchContent_MakeAvailable(imgui_source)

add_library(imgui
        ${imgui_source_SOURCE_DIR}/imgui.cpp
        ${imgui_source_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_source_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_source_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_source_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_source_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_source_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_source_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)


# ========== Find OpenGL ==========
find_package(OpenGL REQUIRED)
# ========== Find GLFW ==========
FetchContent_Declare(GLFW
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)

FetchContent_MakeAvailable(GLFW)

target_link_libraries(imgui PUBLIC glfw)

target_include_directories(imgui PUBLIC
        ${imgui_source_SOURCE_DIR}
)

#target_link_libraries(imgui PUBLIC GLFW)
# ========== Source Files ==========
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
        src/main.cpp
        src/Infrastructure/FileHandle/FileHandle.cpp
        src/Infrastructure/Logging/Logger.cpp
        src/Core/Utils/IDManager.cpp

        src/Domain/ValueObject/*.cpp
        src/Domain/Entity/Budget/*.cpp
        src/Domain/Service/Budget/*.cpp
        src/Domain/Service/Budget/FilterService/*.cpp
        src/Domain/Service/Budget/SortService/*.cpp
        src/Domain/Service/Budget/StatisticService/*.cpp
        src/Domain/Service/Budget/TaxService/*.cpp
        src/Infrastructure/Budget/*.cpp
        src/Application/Budget/*.cpp
        src/Test/*.cpp

        src/Application/Order/*.cpp
        src/Domain/Entity/Order/*.cpp
        src/Domain/Service/Order/*.cpp
        src/Infrastructure/Order/*.cpp

        src/Domain/ValueObject/MealAddon.cpp
        src/Domain/ValueObject/MealIngredients.cpp
        src/Domain/Entity/Meal/Meal.cpp
        src/Domain/Entity/Menu/Menu.cpp
        src/Domain/Entity/Reservation/Reservation.cpp
        src/Domain/Entity/Table/Table.cpp
        src/Domain/Entity/Restaurant/Restaurant.cpp
        src/Application/Meal/MealManager.cpp
        src/Application/Menu/MenuManager.cpp
        src/Application/Reservation/ReservationManager.cpp
        src/Application/Table/TableManager.cpp
        src/Application/Restaurant/RestaurantManager.cpp

        src/Infrastructure/Reservation/IReservationRepository.cpp
        src/Infrastructure/Reservation/FileReservationRepository.cpp

        src/Infrastructure/Table/ITableRepository.cpp
        src/Infrastructure/Table/FileTableRepository.cpp

        src/Infrastructure/Meal/IMealRepository.cpp
        src/Infrastructure/Meal/FileMealRepository.cpp

        src/Infrastructure/Menu/IMenuRepository.cpp
        src/Infrastructure/Menu/FileMenuRepository.cpp

        src/Infrastructure/Restaurant/IRestaurantRepository.cpp
        src/Infrastructure/Restaurant/FileRestaurantRepository.cpp

        src/Domain/Entity/Customer/Customer.cpp
        src/Application/Customer/CustomerManager.cpp
        src/Infrastructure/Customer/CustomerRepository.cpp
        src/Domain/Service/Customer/CustomerService.cpp
        src/Domain/Interface/Person.cpp

        src/Application/Storage/*.cpp
        src/Domain/Entity/*.cpp
        src/Domain/Service/Employee/EmployeeService.cpp
        src/Domain/Entity/Employee/Employee.cpp
        src/Infrastructure/Employee/EmployeeRepository.cpp
        src/Application/Employee/EmployeeManager.cpp

        src/Core/Utils/*.cpp

        # UI
        src/UI/Components/*.cpp
        src/UI/UICore/Core.cpp
)

# ========== Executable ==========
add_executable(RESTAURANT ${SRC_FILES})

# ========== Include Dirs ==========
target_include_directories(RESTAURANT PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Utils

        ${CMAKE_CURRENT_SOURCE_DIR}/src/Application/Budget
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Application/Order

        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity/Meal
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity/Menu
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity/Restaurant
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity/Account
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity/Address
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity/Budget
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity/Order
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity/Employee
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity/Payment
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity/Reservation
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity/Seat
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity/Storage
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Entity/Table

        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Interface
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Observers
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Service
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Service/Budget
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Service/Budget/FilterService
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Service/Budget/SortService
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Service/Budget/TaxService
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Service/Budget/StatisticService
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Service/Order
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/Service/Order
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Domain/ValueObject

        ${CMAKE_CURRENT_SOURCE_DIR}/src/Infrastructure
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Infrastructure/Meal
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Infrastructure/Menu
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Infrastructure/Restaurant
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Infrastructure/Table
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Infrastructure/Reservation
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Infrastructure/FileHandle
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Infrastructure/Logging
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Infrastructure/Budget
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Infrastructure/Order

        ${CMAKE_CURRENT_SOURCE_DIR}/src/UserInterface

        ${CMAKE_CURRENT_SOURCE_DIR}/src/Test

        ${CMAKE_CURRENT_SOURCE_DIR}/external
)

target_link_libraries(RESTAURANT PUBLIC
        imgui
        glfw
        OpenGL::GL
)
# ========== Output Directory ==========
set_target_properties(RESTAURANT PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# ========== Link Libraries ==========
# target_link_libraries(RESTAURANT PRIVATE) # for future use
