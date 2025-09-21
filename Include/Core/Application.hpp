#pragma once

struct ApplicationSettings {
    bool FreedomEnabled = false;
    bool LowerPreRoundRequirement = false;
};

class Application {
public:
    void initialize();
    void uninitialize();

    void run();

    ApplicationSettings& getSettings() {
        return m_settings;
    }

public:
    static Application& getApp() {
        static Application s_application;
        return s_application;
    }

private:
    static constexpr const char* s_consoleTitle = "Debug Console";

private:
    ApplicationSettings m_settings;
    bool m_running = true;

};