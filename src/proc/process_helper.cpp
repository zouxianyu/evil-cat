#include <memory>
#include <windows.h>
#include <TlHelp32.h>
#include "smart_handle.h"
#include "process_helper.h"

std::vector<uint32_t> ProcessHelper::getProcessIdsByName(const std::string &processName) {
    std::vector<uint32_t> found;

    //use the process name to get the process id via CreateToolhelp32Snapshot
    SmartHandle hProcessSnap {CreateToolhelp32Snapshot(
            TH32CS_SNAPPROCESS,
            0
    )};

    if (!hProcessSnap.is_valid()) {
        return {};
    }

    // loop through all the processes and find the one we want
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap.get(), &pe32)) {
        return {};
    }

    do {
        if (processName == pe32.szExeFile) {
            found.push_back(pe32.th32ProcessID);
        }
    } while (Process32Next(hProcessSnap.get(), &pe32));

    return found;
}

bool ProcessHelper::isAlive(uint32_t pid) {
    SmartHandle hProcess {OpenProcess(
            PROCESS_QUERY_INFORMATION,
            FALSE,
            pid
    )};

    if (!hProcess.is_valid()) {
        return false;
    }

    DWORD exitCode;
    if (!GetExitCodeProcess(hProcess.get(), &exitCode)) {
        return false;
    }

    return exitCode == STILL_ACTIVE;
}

uint32_t ProcessHelper::getProcessIdByName(const std::string& processName, size_t index) {
    std::vector<uint32_t> pids = ProcessHelper::getProcessIdsByName(processName);
    if (pids.size() <= index) {
        return 0;
    }
    return pids[index];
}
