
#include "Debug.hpp"

#include <chrono>
#include <cstddef>
#include <fstream>
#include <mutex>
#include <optional>
#include <utility>
#include <algorithm>

namespace SC
{
    Instrumentor *Instrumentor::s_instance = nullptr;

    Instrumentor::Instrumentor()
        : file() {}

    void Instrumentor::OpenFile(std::string path)
    {
        // SC_CORE_INFO("Opeinging");
        std::lock_guard lock(mutex);
        // mutex.lock();
        file.open(path.c_str());
        profileCount = 0;
        WriteHeader();
        fileStartTime = std::chrono::high_resolution_clock::now();

        // mutex.unlock();
    }

    void Instrumentor::WriteDebugFrame(DebugFrame frame)
    {
        // SC_ASSERT(file.is_open(), "Instrumentor::WriteHeader file is not open :3");

        std::lock_guard lock(mutex);

        if (profileCount++ > 0)
            file << ',';

        std::string name = frame.name;
        std::replace(name.begin(), name.end(), '"', '\'');

        file << "{";
        file << "\"cat\":\"function\",";
        file << "\"dur\":" << (frame.duration) << ',';
        file << "\"name\":\"" << name << "\",";
        file << "\"ph\":\"X\",";
        file << "\"pid\":0,";
        file << "\"tid\":" << frame.threadID << ",";
        file << "\"ts\":" << std::chrono::duration_cast<std::chrono::microseconds>(frame.startTime - fileStartTime).count();
        file << "}\n";

        file.flush();

        // mutex.unlock();
    }

    void Instrumentor::CloseFile()
    {
        std::lock_guard lock(mutex);
        WriteFooter();
        file.close();
    }

    void Instrumentor::WriteHeader()
    {
        // mutex.lock();
        // SC_ASSERT(file.is_open(), "Instrumentor::WriteHeader file is not open :3");
        file << "{\"otherData\": {},\"traceEvents\":[\n";
        file.flush();
        // mutex.unlock();
    }

    void Instrumentor::WriteFooter()
    {
        // mutex.lock();
        // SC_ASSERT(file.is_open(), "Instrumentor::WriteHeader file is not open :3");
        file << "\n]}";
        file.flush();
        // mutex.unlock();
    }

    Timer::Timer(std::string name, size_t threadID, bool writeToFile)
        : programName(name),
          start(std::chrono::high_resolution_clock::now()),
          m_threadID(threadID), m_write(writeToFile) {}

    void Timer::EndTimer()
    {
        end = std::chrono::high_resolution_clock::now();
        running = false;
    }

    Timer::~Timer()
    {
        if (running)
            EndTimer();

        if (m_write)
        {
            Instrumentor::Get().WriteDebugFrame({std::chrono::duration<float, std::chrono::microseconds::period>(end - start).count(),
                                                 m_threadID,
                                                 start,
                                                 programName});
        }
    }
}
