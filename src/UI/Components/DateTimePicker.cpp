#include "DateTimePicker.hpp"
#include <imgui.h>
#include <sstream>
#include <iomanip>

DateTimePicker::DateTimePicker() : m_DateTime(DateTime::Now()) {
    UpdateCalendarFromDateTime();
}

DateTimePicker::DateTimePicker(const DateTime &dateTime) : m_DateTime(dateTime) {
    UpdateCalendarFromDateTime();
}

void DateTimePicker::UpdateCalendarFromDateTime() {
    m_CalendarYear = m_DateTime.GetYear();
    m_CalendarMonth = m_DateTime.GetMonth();
}

int DateTimePicker::GetFirstDayOfWeek(int month, int year) {
    // Using Zeller's congruence to find the day of week for the 1st of the month
    // Adjust month and year for Zeller's formula
    int m = month;
    int y = year;
    if (m < 3) {
        m += 12;
        y--;
    }

    int k = y % 100;
    int j = y / 100;

    int dayOfWeek = (1 + (13 * (m + 1)) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;

    // Convert to Sunday = 0, Monday = 1, etc.
    return (dayOfWeek + 6) % 7;
}

std::vector<int> DateTimePicker::GetCalendarDays() const {
    std::vector<int> days;

    int daysInMonth = DateTime::GetDaysInMonth(m_CalendarMonth, m_CalendarYear);
    int firstDay = GetFirstDayOfWeek(m_CalendarMonth, m_CalendarYear);

    // Add empty slots for days before the first day of the month
    for (int i = 0; i < firstDay; i++) {
        days.push_back(0); // 0 represents empty day
    }

    // Add all days of the month
    for (int day = 1; day <= daysInMonth; day++) {
        days.push_back(day);
    }

    return days;
}

void DateTimePicker::RenderCalendar() {
    const char *months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    const char *weekdays[] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};

    // Month/Year navigation
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 4));

    if (ImGui::Button("<")) {
        m_CalendarMonth--;
        if (m_CalendarMonth < 1) {
            m_CalendarMonth = 12;
            m_CalendarYear--;
        }
    }

    ImGui::SameLine();
    // ImGui::Text("%9s", months[m_CalendarMonth - 1], m_CalendarYear);
    int monthIndex = m_CalendarMonth - 1;
    ImGui::SetNextItemWidth(ImGui::CalcTextSize("September  ").x);
    if (ImGui::BeginCombo("##month", months[monthIndex],
                          ImGuiComboFlags_NoArrowButton)) {
        for (int n = 0; n < IM_ARRAYSIZE(months); n++) {
            bool isSelected = (monthIndex == n);
            if (ImGui::Selectable(months[n], isSelected)) {
                m_CalendarMonth = n + 1;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();

    if (ImGui::Button(">")) {
        m_CalendarMonth++;
        if (m_CalendarMonth > 12) {
            m_CalendarMonth = 1;
            m_CalendarYear++;
        }
    }

    // Year input
    ImGui::SameLine();
    ImGui::PushItemWidth(60);
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 70);
    if (ImGui::InputInt("##year", &m_CalendarYear, 0, 0)) {
        if (m_CalendarYear < 1) m_CalendarYear = 1;
    }
    ImGui::PopItemWidth();

    ImGui::Separator();

    // Calendar grid
    if (!ImGui::BeginTable("calendar", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::PopStyleVar();
        return;
    }
    // Header row with weekdays
    for (const char *day: weekdays) {
        ImGui::TableSetupColumn(day, ImGuiTableColumnFlags_WidthStretch);
    }
    ImGui::Separator();

    // Calendar days
    std::vector<int> calendarDays = GetCalendarDays();
    int currentDay = m_DateTime.GetDay();
    int currentMonth = m_DateTime.GetMonth();
    int currentYear = m_DateTime.GetYear();

    for (size_t i = 0; i < calendarDays.size(); i++) {
        if (i % 7 == 0) {
            ImGui::TableNextRow();
        }
        ImGui::TableSetColumnIndex(i % 7);
        int day = calendarDays[i];

        if (day == 0) {
            // Empty cell
            ImGui::Text(" ");
        } else {
            // Style current selected day
            bool isSelected = (day == currentDay &&
                               m_CalendarMonth == currentMonth &&
                               m_CalendarYear == currentYear);

            std::string dayStr = std::to_string(day);
            if (ImGui::Selectable(dayStr.c_str(), isSelected, ImGuiSelectableFlags_DontClosePopups)) {
                try {
                    // Update the DateTime with the selected day
                    m_DateTime = DateTime(day, m_CalendarMonth, m_CalendarYear,
                                          m_DateTime.GetHour(), m_DateTime.GetMinute(), m_DateTime.GetSecond());
                } catch (const std::invalid_argument &) {
                    // If invalid date, don't update
                }
            }
        }
    }
    ImGui::PopStyleVar();
    ImGui::EndTable();
}

void DateTimePicker::RenderTimeControls() {
    ImGui::Text("Time:");

    static int hour = m_DateTime.GetHour();
    static int minute = m_DateTime.GetMinute();
    static int second = m_DateTime.GetSecond();

    ImGui::PushItemWidth(120);

    if (ImGui::InputInt("##hour", &hour, 1, 1)) {
        try {
            m_DateTime.SetHour(hour);
        } catch (const std::invalid_argument &) {
            // Invalid hour, don't update
        }
    }
    ImGui::SameLine();
    ImGui::Text(":");
    ImGui::SameLine();

    if (ImGui::InputInt("##minute", &minute, 1, 1)) {
        try {
            m_DateTime.SetMinute(minute);
        } catch (const std::invalid_argument &) {
            // Invalid minute, don't update
        }
    }

    if (m_ShowSeconds) {
        ImGui::SameLine();
        ImGui::Text(":");
        ImGui::SameLine();

        if (ImGui::InputInt("##second", &second, 1, 1)) {
            try {
                m_DateTime.SetSecond(second);
            } catch (const std::invalid_argument &) {
                // Invalid second, don't update
            }
        }
    }
    ImGui::PopItemWidth();
}

bool DateTimePicker::Render(const char *label) {
    bool changed = false;

    if (!ImGui::BeginPopup(label)) {
        return false;
    }

    ImGui::Text("Select Date & Time");
    ImGui::Separator();

    // Calendar for date selection
    RenderCalendar();

    // Time selection (if enabled)
    if (m_ShowTime) {
        ImGui::Separator();
        RenderTimeControls();
    }

    ImGui::Separator();

    // Display current selection
    ImGui::Text("Selected: %s", GetDateTimeString().c_str());

    ImGui::Spacing();
    if (ImGui::Button("OK", ImVec2(60, 0))) {
        ImGui::CloseCurrentPopup();
        changed = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(60, 0))) {
        ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Now", ImVec2(60, 0))) {
        m_DateTime = DateTime::Now();
        UpdateCalendarFromDateTime();
        changed = true;
    }

    ImGui::EndPopup();
    return changed;
}

bool DateTimePicker::RenderDateOnly(const char *label) {
    bool changed = false;

    ImGui::Text("%s", label);
    ImGui::SameLine();

    std::string dateStr = GetDateString();
    ImGui::PushItemWidth(100);
    if (ImGui::Button(dateStr.c_str())) {
        ImGui::OpenPopup("DatePicker");
    }
    ImGui::PopItemWidth();

    if (ImGui::BeginPopup("DatePicker")) {
        ImGui::Text("Select Date");
        ImGui::Separator();

        RenderCalendar();

        ImGui::Separator();
        ImGui::Text("Selected: %s", GetDateString().c_str());

        if (ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
            changed = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Today")) {
            DateTime now = DateTime::Now();
            try {
                m_DateTime = DateTime(now.GetDay(), now.GetMonth(), now.GetYear(),
                                      m_DateTime.GetHour(), m_DateTime.GetMinute(), m_DateTime.GetSecond());
                UpdateCalendarFromDateTime();
                changed = true;
            } catch (const std::invalid_argument &) {
                // Handle invalid date
            }
        }

        ImGui::EndPopup();
    }

    return changed;
}

bool DateTimePicker::RenderTimeOnly(const char *label) {
    bool changed = false;

    ImGui::Text("%s", label);
    ImGui::SameLine();

    std::string timeStr = GetTimeString();
    ImGui::PushItemWidth(80);
    if (ImGui::Button(timeStr.c_str())) {
        ImGui::OpenPopup("TimePicker");
    }
    ImGui::PopItemWidth();

    if (ImGui::BeginPopup("TimePicker")) {
        ImGui::Text("Select Time");
        ImGui::Separator();

        RenderTimeControls();

        ImGui::Separator();
        ImGui::Text("Selected: %s", GetTimeString().c_str());

        if (ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
            changed = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Now")) {
            DateTime now = DateTime::Now();
            try {
                m_DateTime.SetHour(now.GetHour());
                m_DateTime.SetMinute(now.GetMinute());
                m_DateTime.SetSecond(now.GetSecond());
                changed = true;
            } catch (const std::invalid_argument &) {
                // Handle invalid time
            }
        }

        ImGui::EndPopup();
    }

    return changed;
}

bool DateTimePicker::RenderInline(const char *label) {
    ImGui::Text("%s", label);

    // Render inline calendar (simplified)
    RenderCalendar();

    // Time controls (if enabled)
    if (m_ShowTime) {
        ImGui::Spacing();
        RenderTimeControls();
    }

    return false; // Always return false for inline as it doesn't have OK/Cancel
}

void DateTimePicker::SetDateTime(const DateTime &dateTime) {
    m_DateTime = dateTime;
    UpdateCalendarFromDateTime();
}

std::string DateTimePicker::GetDateString() const {
    return m_DateTime.ToStringDate();
}

std::string DateTimePicker::GetTimeString() const {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << m_DateTime.GetHour() << ":"
            << std::setw(2) << m_DateTime.GetMinute();

    if (m_ShowSeconds) {
        oss << ":" << std::setw(2) << m_DateTime.GetSecond();
    }

    return oss.str();
}

std::string DateTimePicker::GetDateTimeString() const {
    if (m_ShowTime) {
        return m_DateTime.ToStringDateTime();
    }
    return m_DateTime.ToStringDate();
}

// Static utility method for simple datetime picking
bool DateTimePicker::DateTimePopup(const char *label, DateTime &dateTime, bool showTime, bool showSeconds) {
    static DateTimePicker picker;
    bool changed = false;

    std::string buttonText = dateTime.ToStringDateTime();
    if (ImGui::Button(buttonText.c_str())) {
        picker.SetDateTime(dateTime);
        picker.SetShowTime(showTime);
        picker.SetShowSeconds(showSeconds);
        ImGui::OpenPopup(label);
    }

    if (picker.Render(label)) {
        dateTime = picker.GetDateTime();
        changed = true;
    }

    return changed;
}
