#include <windows.h>
#include <TlHelp32.h>
#include "process_info_external.h"

ProcessInfo &ProcessInfoExternal::getInstance() {
    static ProcessInfoExternal instance;
    return instance;
}

bool ProcessInfoExternal::attach(const std::string &processName) {
    detach();

    if (processName.empty()) {
        return false;
    }

    DWORD pid = getProcessIdByName(processName);
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == nullptr || hProcess == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}

bool ProcessInfoExternal::detach() {
    if (hProcess == nullptr || hProcess == INVALID_HANDLE_VALUE) {
        return false;
    }

    CloseHandle(hProcess);
    hProcess = nullptr;
    return true;
}

bool ProcessInfoExternal::getModuleAddress(const std::string &moduleName, void *&address) {
    if (hProcess == nullptr || hProcess == INVALID_HANDLE_VALUE) {
        return false;
    }

    // if the module is already recorded, return its base address
    auto it = modulesAddress.find(moduleName);
    if (it != modulesAddress.end()) {
        address = it->second;
        return true;
    }

    // otherwise, find the module's base address by enumerating all modules
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetProcessId(hProcess));
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (moduleName == modEntry.szModule)
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    address = reinterpret_cast<void*>(modBaseAddr);
                    modulesAddress[moduleName] = address;
                    CloseHandle(hSnap);
                    return true;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return false;
}

bool ProcessInfoExternal::refresh() {
    modulesAddress.clear();
    return true;
}

DWORD ProcessInfoExternal::getProcessIdByName(const std::string &processName) {
    //use the process name to get the process id via CreateToolhelp32Snapshot
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return 0;
    }
    // loop through all the processes and find the one we want
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return 0;
    }
    do {
        if (processName == pe32.szExeFile) {
            CloseHandle(hProcessSnap);
            return pe32.th32ProcessID;
        }
    } while (Process32Next(hProcessSnap, &pe32));
    return 0;

}
