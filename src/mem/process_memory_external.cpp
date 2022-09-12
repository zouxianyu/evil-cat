#include <windows.h>
#include <TlHelp32.h>
#include "process_memory_external.h"

ProcessMemory &ProcessMemoryExternal::getInstance() {
    static ProcessMemoryExternal instance;
    return instance;
}

bool ProcessMemoryExternal::attach(const std::string &processName) {
    detach();

    if (processName.empty()) {
        return false;
    }

    DWORD pid = getProcessIdByName(processName);
    hProcess = OpenProcess(
            PROCESS_ALL_ACCESS,
            FALSE,
            pid
    );
    if (hProcess == nullptr || hProcess == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}

bool ProcessMemoryExternal::detach() {
    if (hProcess == nullptr || hProcess == INVALID_HANDLE_VALUE) {
        return false;
    }

    CloseHandle(hProcess);
    hProcess = nullptr;
    return true;
}

bool ProcessMemoryExternal::read(void *address, void *buffer, size_t size) {
    if (hProcess == nullptr || hProcess == INVALID_HANDLE_VALUE) {
        return false;
    }

    SIZE_T bytesRead;
    if (!ReadProcessMemory(
            hProcess,
            address,
            buffer,
            size,
            &bytesRead
    )) {
        return false;
    }

    return bytesRead == size;
}

bool ProcessMemoryExternal::write(void *address, const void *buffer, size_t size) {
    if (hProcess == nullptr || hProcess == INVALID_HANDLE_VALUE) {
        return false;
    }

    SIZE_T bytesWritten;
    if (!WriteProcessMemory(
            hProcess,
            address,
            buffer,
            size,
            &bytesWritten
    )) {
        return false;
    }

    return bytesWritten == size;
}

DWORD ProcessMemoryExternal::getProcessIdByName(const std::string& processName) {
    //use the process name to get the process id via CreateToolhelp32Snapshot
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(
            TH32CS_SNAPPROCESS,
            0
    );
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
