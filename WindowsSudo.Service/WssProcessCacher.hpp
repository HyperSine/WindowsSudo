#pragma once
#include <windows.h>
#include <mutex>
#include <unordered_map>

namespace WSS::Internal {

    class ProcessCacher {
    private:

        using IdType = DWORD;

        struct ContextType {
            ULONGLONG CreateTime;
            HANDLE ProcessHandle;
        };

        std::mutex m_Lock;
        std::unordered_map<IdType, ContextType> m_Database;

        ProcessCacher() = default;

    public:

        static ProcessCacher& GetInstance() noexcept;

        void Insert(DWORD dwProcessId, HANDLE hProcess);
        
        bool Query(DWORD dwProcessId);

        ~ProcessCacher();
    };

}

