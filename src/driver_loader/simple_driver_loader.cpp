#include <windows.h>
#include <winternl.h>
#include "simple_driver_loader.h"

namespace nt {
    typedef NTSTATUS(*NtLoadDriver)(PUNICODE_STRING DriverServiceName);
    typedef NTSTATUS(*NtUnloadDriver)(PUNICODE_STRING DriverServiceName);
    typedef NTSTATUS(*RtlAdjustPrivilege)(_In_ ULONG Privilege, _In_ BOOLEAN Enable, _In_ BOOLEAN Client, _Out_ PBOOLEAN WasEnabled);
}

bool SimpleDriverLoader::load(const std::string &path, const std::string &name) {
    const static DWORD ServiceTypeKernel = 1;
    const std::wstring driver_name {name.begin(), name.end()};
    const std::wstring servicesPath = L"SYSTEM\\CurrentControlSet\\Services\\" + driver_name;
    const std::wstring nPath = L"\\??\\" + std::wstring(path.begin(), path.end());

    HKEY dservice;
    LSTATUS status = RegCreateKeyW(HKEY_LOCAL_MACHINE, servicesPath.c_str(), &dservice); //Returns Ok if already exists
    if (status != ERROR_SUCCESS) {
        return false;
    }

    status = RegSetKeyValueW(dservice, NULL, L"ImagePath", REG_EXPAND_SZ, nPath.c_str(), (DWORD)(nPath.size()*sizeof(wchar_t)));
    if (status != ERROR_SUCCESS) {
        RegCloseKey(dservice);
        return false;
    }

    status = RegSetKeyValueW(dservice, NULL, L"Type", REG_DWORD, &ServiceTypeKernel, sizeof(DWORD));
    if (status != ERROR_SUCCESS) {
        RegCloseKey(dservice);
        return false;
    }

    RegCloseKey(dservice);

    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (ntdll == NULL) {
        return false;
    }

    auto RtlAdjustPrivilege = (nt::RtlAdjustPrivilege)GetProcAddress(ntdll, "RtlAdjustPrivilege");
    auto NtLoadDriver = (nt::NtLoadDriver)GetProcAddress(ntdll, "NtLoadDriver");

    ULONG SE_LOAD_DRIVER_PRIVILEGE = 10UL;
    BOOLEAN SeLoadDriverWasEnabled;
    NTSTATUS Status = RtlAdjustPrivilege(SE_LOAD_DRIVER_PRIVILEGE, TRUE, FALSE, &SeLoadDriverWasEnabled);
    if (!NT_SUCCESS(Status)) {
        return false;
    }

    std::wstring wdriver_reg_path = L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\" + driver_name;
    UNICODE_STRING serviceStr;
    RtlInitUnicodeString(&serviceStr, wdriver_reg_path.c_str());

    Status = NtLoadDriver(&serviceStr);

    //Never should occur since kdmapper checks for "IsRunning" driver before
    if (Status == 0xC000010E) {// STATUS_IMAGE_ALREADY_LOADED
        return true;
    }

    return NT_SUCCESS(Status);
}

bool SimpleDriverLoader::unload(const std::string &name) {
    std::wstring driver_name {name.begin(), name.end()};

    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (ntdll == NULL)
        return false;

    std::wstring wdriver_reg_path = L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\" + driver_name;
    UNICODE_STRING serviceStr;
    RtlInitUnicodeString(&serviceStr, wdriver_reg_path.c_str());

    HKEY driver_service;
    std::wstring servicesPath = L"SYSTEM\\CurrentControlSet\\Services\\" + driver_name;
    LSTATUS status = RegOpenKeyW(HKEY_LOCAL_MACHINE, servicesPath.c_str(), &driver_service);
    if (status != ERROR_SUCCESS) {
        if (status == ERROR_FILE_NOT_FOUND) {
            return true;
        }
        return false;
    }
    RegCloseKey(driver_service);

    auto NtUnloadDriver = (nt::NtUnloadDriver)GetProcAddress(ntdll, "NtUnloadDriver");
    NTSTATUS st = NtUnloadDriver(&serviceStr);
    if (st != 0x0) {
        status = RegDeleteKeyW(HKEY_LOCAL_MACHINE, servicesPath.c_str());
        return false; //lets consider unload fail as error because can cause problems with anti cheats later
    }


    status = RegDeleteKeyW(HKEY_LOCAL_MACHINE, servicesPath.c_str());
    if (status != ERROR_SUCCESS) {
        return false;
    }
    return true;
}


