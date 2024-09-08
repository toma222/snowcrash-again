
#pragma once

#include <snowcrash/core/Core.hpp>
#include <cstddef>
#include <fstream>
#include <mutex>
#include <optional>
#include <chrono>

namespace SC
{
    // this might give errors, but it works lol
    using timePoint = std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::_V2::system_clock::duration>;

    struct DebugFrame
    {
        float duration;
        size_t threadID;
        timePoint startTime;
        std::string name;
    };

    class Instrumentor
    {
    public:
        Instrumentor(const Instrumentor &) = delete;
        void OpenFile(std::string path);
        void WriteDebugFrame(DebugFrame frame);
        void CloseFile();
        static Instrumentor &Get()
        {
            if (s_instance == nullptr)
                s_instance = new Instrumentor();
            return *s_instance;
        }

    private:
        void WriteHeader();
        void WriteFooter();
        Instrumentor();

    private:
        std::ofstream file;
        std::mutex mutex;
        int profileCount{0};
        static Instrumentor *s_instance;
        timePoint fileStartTime;
    };

    struct Timer
    {
    public:
        Timer(std::string name, size_t threadID = 0, bool writeToFile = true);
        void EndTimer();
        float GetMicroseconds() { return std::chrono::duration<float, std::chrono::microseconds::period>(end - start).count(); };
        ~Timer();

    private:
        std::string programName{"_null"};
        timePoint start;
        timePoint end;
        bool running{true};
        size_t m_threadID;
        const bool m_write;
    };
}

#define SC_INSTRUMENT_TIMER(...) ::SC::Timer KN_timer(__VA_ARGS__);
#define SC_INSTRUMENT_NEW_FILE(name) ::SC::Instrumentor::Get().OpenFile(name);
#define SC_INSTRUMENT_CLOSE_FILE() ::SC::Instrumentor::Get().CloseFile();

// #define KN_INSTRUMENT_FUNCTION(func, threadID) KN_INSTRUMENT_TIMER(__func__, threadID)
#define SC_INSTRUMENT_SCOPE(...) SC_INSTRUMENT_TIMER(__VA_ARGS__)
#define SC_INSTRUMENT_FUNCTION() SC_INSTRUMENT_TIMER(__func__, 0)
