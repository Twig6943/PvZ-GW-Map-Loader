#pragma once

#include <memory>

class Application {
public:
    void initialize();
    void uninitialize();

    void run();

private:
    static constexpr const char* s_consoleTitle = "Debug Console";

private:
    bool m_running = true;

};

inline std::unique_ptr<Application> g_application;
