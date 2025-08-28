#include "Core.hpp"

#include <stdexcept>

#include "Constants.hpp"
#include "FileMenuRepository.hpp"
#include "IDManager.hpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "UI/Components/Dashboard.hpp"
#include "UI/Components/ReservationScreen.hpp"

#include "../Components/StorageScreen.hpp"
#include "UI/Components/BudgetScreen.hpp"
#include "UI/Components/EmployeesScreen.hpp"
#include "UI/Components/MenuScreen.hpp"
#include "UI/Components/OrderScreen.hpp"

static void glfw_error_callback(int error, const char *description) {
    throw std::runtime_error("GLFW Error " + std::to_string(error) + ": " + description);
}

void Core::Init() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    this->m_ScaleFactor = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    this->m_Window = glfwCreateWindow(Constants::WINDOW_WIDTH * this->m_ScaleFactor,
                                      Constants::WINDOW_HEIGHT * this->m_ScaleFactor,
                                      "Restaurant Management Dashboard", nullptr, nullptr);

    if (!this->m_Window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(this->m_Window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    this->m_IO = &ImGui::GetIO();

    // ------------------ FONT SETUP ------------------
    ImFont *defaultFont = this->m_IO->Fonts->AddFontFromFileTTF(
        "assets/fonts/segoeui.ttf", // change path to your font
        18.0f * this->m_ScaleFactor // size (scaled with DPI)
    );
    if (!defaultFont) {
        throw std::runtime_error("Failed to load font!");
    }
    this->m_IO->FontDefault = defaultFont;

    // ------------------ STYLE SETUP ------------------
    ImGui::StyleColorsDark(); // start with dark theme
    this->m_Style = &ImGui::GetStyle();
    ImGuiStyle &style = *this->m_Style;

    // Rounding
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 6.0f;

    // Spacing & padding
    style.FramePadding = ImVec2(10, 6);
    style.ItemSpacing = ImVec2(10, 8);
    style.WindowPadding = ImVec2(12, 12);

    // Scale for high-DPI
    style.ScaleAllSizes(this->m_ScaleFactor);
    this->m_IO->FontGlobalScale = this->m_ScaleFactor;

    // ------------------ COLOR CUSTOMIZATION ------------------
    ImVec4 *colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.11f, 0.13f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.35f, 0.60f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.45f, 0.70f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.35f, 0.55f, 1.0f);

    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.45f, 0.75f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.55f, 0.85f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.35f, 0.65f, 1.0f);

    colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.25f, 0.30f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.30f, 0.35f, 1.0f);

    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.25f, 0.40f, 1.0f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.35f, 0.55f, 1.0f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.35f, 0.60f, 1.0f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.18f, 0.28f, 1.0f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.16f, 0.25f, 0.40f, 1.0f);


    this->m_IO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    this->m_IO->DisplaySize = ImVec2(Constants::WINDOW_WIDTH * this->m_ScaleFactor,
                                     Constants::WINDOW_HEIGHT * this->m_ScaleFactor);

    ImGui::StyleColorsDark();
    this->m_Style = &ImGui::GetStyle();
    this->m_Style->ScaleAllSizes(this->m_ScaleFactor);
    this->m_Style->FontScaleDpi = this->m_ScaleFactor;

    ImGui_ImplGlfw_InitForOpenGL(this->m_Window, true);
    ImGui_ImplOpenGL3_Init(Constants::GLSL_VERSION.c_str());

    // IDManager::Init("Data/IDRegistry.json");
    // static FileReservationRepository reservationRepo("Reservation.json");
    // static ReservationManager reservationManager(reservationRepo);
    // PushScreen(std::make_unique<ReservationScreen>(*this, reservationManager, reservationRepo));
    PushScreen(std::make_unique<StorageScreen>(*this));
    // PushScreen(std::make_unique<EmployeesScreen>(*this));
    // PushScreen(std::make_unique<MenuScreen>(*this));
    // FileMenuRepository menuRepo("Data/Menu.json", "Data/MenuItem.json", "Data/MenuAddons.json");;
    // std::shared_ptr mealRepo(
    //     std::make_shared<FileMealRepository>("Data/MealHistory.json", menuRepo));
    // std::unique_ptr orderRepo(std::make_unique<OrderRepository>("Data/Order.json", mealRepo));
    // std::unique_ptr orderService = std::make_unique<OrderService>();
    // std::shared_ptr orderManager(std::make_shared<OrderManager>(orderRepo, orderService));
    // PushScreen(std::make_unique<OrderScreen>(*this, orderManager, mealRepo));

    // Initialize Dashboard as the main screen
    PushScreen(std::make_unique<Dashboard>(*this));
}


void Core::Start() {
    assert(this->m_Window != nullptr);
    while (!glfwWindowShouldClose(this->m_Window) && !this->m_ScreenStack.empty()) {
        glfwPollEvents();

        if (glfwGetWindowAttrib(this->m_Window, GLFW_ICONIFIED) != 0) {
            // If the window is minimized, skip rendering
            ImGui_ImplGlfw_Sleep(Constants::SLEEPTIME);
            continue;
        }

        Update();
    }
}

void Core::Shutdown() {
    while (this->m_ScreenStack.empty() == false) {
        this->m_ScreenStack.back()->OnExit();
        this->m_ScreenStack.pop_back();
    }
    if (this->m_Window != nullptr) {
        return;
    }
    if (!glfwWindowShouldClose(this->m_Window)) {
        return;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(this->m_Window);
    glfwTerminate();
    this->m_Window = nullptr;
    this->m_IO = nullptr;
    this->m_Context = nullptr;
    this->m_Style = nullptr;
}

void Core::TryPopScreen() {
    if (this->m_ShouldPop && this->m_ScreenStack.size() > 1) {
        this->m_ShouldPop = false;
        this->m_ScreenStack.back()->OnExit();
        this->m_ScreenStack.pop_back();
        if (this->m_ShouldChangeState) {
            this->m_ShouldChangeState = false;
            PushScreen(std::move(this->m_ChangedScreen));
        }
    }
}

ImGuiIO &Core::GetIO() const {
    assert(this->m_IO != nullptr);
    return *this->m_IO;
}

ImGuiContext &Core::GetContext() const {
    assert(this->m_Context != nullptr);
    return *this->m_Context;
}

ImGuiStyle &Core::GetStyle() const {
    assert(this->m_Style != nullptr);
    return *this->m_Style;
}

GLFWwindow *Core::GetWindow() const {
    assert(this->m_Window != nullptr);
    return this->m_Window;
}

Screen &Core::GetCurrentScreen() const {
    return *m_ScreenStack.back();
}

void Core::PushScreen(std::unique_ptr<Screen> &&screen) {
    this->m_ScreenStack.emplace_back(std::move(screen));
    this->m_ScreenStack.back()->Init();
}

void Core::PopScreen() {
    this->m_ShouldPop = true;
}

void Core::Update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(this->m_IO->DisplaySize);

    const auto windowBorderSize = ImGui::GetStyle().WindowBorderSize;
    const auto windowRounding = ImGui::GetStyle().WindowRounding;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("Content", nullptr, GetWindowFlags());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, windowBorderSize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, windowRounding);

    Screen &currentScreen = this->GetCurrentScreen();
    currentScreen.Render(this->m_IO->DeltaTime);

    ImGui::PopStyleVar(2);
    ImGui::End();
    ImGui::PopStyleVar(2);

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(this->m_Window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    glClearColor(Constants::CLEAR_COLOR.Value.x,
                 Constants::CLEAR_COLOR.Value.y,
                 Constants::CLEAR_COLOR.Value.z,
                 Constants::CLEAR_COLOR.Value.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(this->m_Window);

    TryPopScreen();
}

ImGuiWindowFlags Core::GetWindowFlags() {
    return ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
           ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
}
