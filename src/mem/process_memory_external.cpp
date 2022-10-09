#include <windows.h>
#include <TlHelp32.h>
#include "process_memory_external.h"

NTSTATUS(NTAPI *NtWow64ReadVirtualMemory64)(
        IN  HANDLE   ProcessHandle,
        IN  ULONG64  BaseAddress,
        OUT PVOID    BufferData,
        IN  ULONG64  BufferLength,
        OUT PULONG64 ReturnLength OPTIONAL);

NTSTATUS(NTAPI *NtWow64WriteVirtualMemory64)(
        IN  HANDLE   ProcessHandle,
        IN  ULONG64  BaseAddress,
        OUT PVOID    BufferData,
        IN  ULONG64  BufferLength,
        OUT PULONG64 ReturnLength OPTIONAL);


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

bool ProcessMemoryExternal::read(gameptr_t address, void *buffer, gamesize_t size) {
    if (hProcess == nullptr || hProcess == INVALID_HANDLE_VALUE) {
        return false;
    }

    // if we are 32 bit process and we want to read a 64 bit address
    // we need another function
    if constexpr (sizeof(void *) == 4 && sizeof(gameptr_t) == 8) {
        return (NtWow64ReadVirtualMemory64(
                hProcess,
                address,
                buffer,
                size,
                nullptr
        )) >= 0;
    } else {
        return ReadProcessMemory(
                hProcess,
                reinterpret_cast<void *>(address),
                buffer,
                size,
                nullptr
        );
    }
}

bool ProcessMemoryExternal::write(gameptr_t address, const void *buffer, gamesize_t size) {
    if (hProcess == nullptr || hProcess == INVALID_HANDLE_VALUE) {
        return false;
    }

    if constexpr (sizeof(void *) == 4 && sizeof(gameptr_t) == 8) {
        return (NtWow64WriteVirtualMemory64(
                hProcess,
                address,
                const_cast<void *>(buffer),
                size,
                nullptr
        )) >= 0;
    } else {
        return WriteProcessMemory(
                hProcess,
                reinterpret_cast<void *>(address),
                buffer,
                size,
                nullptr
        );
    }
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
