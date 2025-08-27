#pragma once
#include "../../Domain/ValueObject/DateTime.hpp"
#include <imgui.h>
#include <string>
#include <vector>

class DateTimePicker {
private:
    DateTime m_DateTime;

    bool m_ShowTime = true;
    bool m_ShowSeconds = true;

    // Calendar navigation
    int m_CalendarYear;
    int m_CalendarMonth;

    // Helper methods
    void UpdateCalendarFromDateTime();

    void RenderCalendar();

    void RenderTimeControls();

    std::vector<int> GetCalendarDays() const;

    static int GetFirstDayOfWeek(int month, int year); // 0 = Sunday, 1 = Monday, etc.

public:
    DateTimePicker();

    DateTimePicker(const DateTime &dateTime);

    // Main render method
    bool Render(const char *label);

    // Render variants
    bool RenderDateOnly(const char *label);

    bool RenderTimeOnly(const char *label);

    bool RenderInline(const char *label);

    // Settings
    void SetShowTime(bool show) { m_ShowTime = show; }
    void SetShowSeconds(bool show) { m_ShowSeconds = show; }
    bool GetShowTime() const { return m_ShowTime; }
    bool GetShowSeconds() const { return m_ShowSeconds; }

    // Get/Set DateTime
    const DateTime &GetDateTime() const { return m_DateTime; }

    void SetDateTime(const DateTime &dateTime);

    // Utility methods
    std::string GetDateString() const;

    std::string GetTimeString() const;

    std::string GetDateTimeString() const;

    // Static utility method for simple datetime picking
    static bool DateTimePopup(const char *label, DateTime &dateTime, bool showTime = true, bool showSeconds = false);
};
