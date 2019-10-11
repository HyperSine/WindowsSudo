#include "WssProcessCacher.hpp"
#include "WscRuntime.hpp"
#include <system_error>

namespace WSS::Internal {

    ProcessCacher& ProcessCacher::GetInstance() noexcept {
        static ProcessCacher Instance;
        return Instance;
    }

    void ProcessCacher::Insert(DWORD dwProcessId, HANDLE hProcess) {
        std::lock_guard Guard(m_Lock);

        auto it = m_Database.find(dwProcessId);
        if (it == m_Database.end()) {
            m_Database.insert(std::pair<IdType, ContextType>{ dwProcessId, { GetTickCount64(), hProcess } });
        } else {
            throw std::system_error(ERROR_ALREADY_EXISTS, std::system_category());
        }
    }

    bool ProcessCacher::Query(DWORD dwProcessId) {
        std::lock_guard Guard(m_Lock);

        auto it = m_Database.find(dwProcessId);
        if (it == m_Database.end()) {
            return false;
        }

        if ((GetTickCount64() - it->second.CreateTime <= 5 * 60 * 1000) && WSC::RtlProcessIsExited(it->second.ProcessHandle) == FALSE) {
            return true;
        } else {
            CloseHandle(it->second.ProcessHandle);
            m_Database.erase(it);
            return false;
        }
    }

    ProcessCacher::~ProcessCacher() {
        std::lock_guard Guard(m_Lock);

        for (auto& it : m_Database) {
            CloseHandle(it.second.ProcessHandle);
        }
    }
}

