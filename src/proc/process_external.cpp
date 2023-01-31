#include <windows.h>
#include <TlHelp32.h>
#include "process_helper.h"
#include "process_external.h"

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

bool ProcessExternal::attach(uint32_t pid) {
    detach();

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

bool ProcessExternal::detach() {
    if (hProcess == nullptr || hProcess == INVALID_HANDLE_VALUE) {
        return false;
    }

    CloseHandle(hProcess);
    hProcess = nullptr;
    return true;
}

gameptr_t ProcessExternal::getModuleAddress(const std::string &moduleName) {
    if (hProcess == nullptr || hProcess == INVALID_HANDLE_VALUE) {
        return 0;
    }

    HANDLE hSnap = CreateToolhelp32Snapshot(
            TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,
            GetProcessId(hProcess)
    );

    // invalid snapshot
    if (hSnap == nullptr || hSnap == INVALID_HANDLE_VALUE) {
        return 0;
    }

    // try to find the module address
    MODULEENTRY32 modEntry;
    modEntry.dwSize = sizeof(modEntry);
    if (Module32First(hSnap, &modEntry))
    {
        do
        {
            if (moduleName == modEntry.szModule)
            {
                gameptr_t modBaseAddr = reinterpret_cast<gameptr_t>(modEntry.modBaseAddr);
                CloseHandle(hSnap);
                return modBaseAddr;
            }
        } while (Module32Next(hSnap, &modEntry));
    }

    // not found
    CloseHandle(hSnap);
    return 0;
}

bool ProcessExternal::read(gameptr_t address, void *buffer, gamesize_t size) {
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

bool ProcessExternal::write(gameptr_t address, const void *buffer, gamesize_t size) {
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
