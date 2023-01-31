#include <string>
#include <filesystem>
#include <thread>
#include <cstdint>
#include <windows.h>
#include <tlhelp32.h>
#include "packet.h"
#include "smart_handle.h"
#include "draw.h"

struct MapDeleter {
    void operator()(PVOID p) {
        if (p) {
            UnmapViewOfFile(p);
        }
    }
};

using SmartControlBlock = std::unique_ptr<Comm::ControlBlock, MapDeleter>;

static const char *const dllName = "dwm_server.dll";
static bool initialized = false;
static SmartHandle fileMappingHandle = nullptr;
static SmartControlBlock controlBlock = nullptr;

static uint8_t drawingId = 0;
static uintptr_t drawingPtr = 0;

static void enableDebugPrivilege() {
    HANDLE hToken;
    LUID sedebugnameValue;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ALL_ACCESS, &hToken)) {
        return;
    }

    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue)) {
        CloseHandle(hToken);
        return;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = sedebugnameValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp,
                               sizeof tkp, NULL, NULL)) {
        CloseHandle(hToken);
    }
}

static bool injectDll(uint32_t pid, const std::string &path) {
    HANDLE hProcess = nullptr;
    LPVOID address = nullptr;
    DWORD n = 0;
    PVOID remoteLoadLibrary = nullptr;
    HANDLE hThread = nullptr;
    bool result = true;

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == nullptr) {
        goto error;
    }

    address = VirtualAllocEx(
            hProcess,
            nullptr,
            path.size(),
            MEM_COMMIT | MEM_RESERVE,
            PAGE_READWRITE
    );

    if (address == nullptr) {
        goto error;
    }

    n = WriteProcessMemory(
            hProcess,
            address,
            path.c_str(),
            path.size(),
            nullptr
    );

    if (n == 0) {
        goto error;
    }

    remoteLoadLibrary = GetProcAddress(GetModuleHandle("kernel32.dll"),
                                       "LoadLibraryA");
    if (remoteLoadLibrary == nullptr) {
        goto error;
    }

    hThread = CreateRemoteThread(
            hProcess,
            nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(remoteLoadLibrary),
            address,
            0,
            nullptr
    );
    if (hThread == nullptr) {
        goto error;
    }

    WaitForSingleObject(hThread, INFINITE);

    cleanup:
    if (hThread != nullptr) {
        CloseHandle(hThread);
    }
    if (address != nullptr) {
        VirtualFreeEx(hProcess, address, 0, MEM_RELEASE);
    }
    if (hProcess != nullptr) {
        CloseHandle(hProcess);
    }
    return result;

    error:
    result = false;
    goto cleanup;
}

static uint32_t getPidByName(const std::string &processName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(snapshot, &entry) == TRUE) {
        while (Process32Next(snapshot, &entry) == TRUE) {
            if (processName == entry.szExeFile) {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        }
    }
    CloseHandle(snapshot);
    return 0;
}

static bool fileMappingInitialize() {
    fileMappingHandle.reset(OpenFileMapping(
            FILE_MAP_ALL_ACCESS,
            FALSE,
            Comm::FILE_MAPPING_NAME
    ));
    if (!fileMappingHandle) {
        return false;
    }
    controlBlock.reset(reinterpret_cast<Comm::ControlBlock *>(MapViewOfFile(
            fileMappingHandle.get(),
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            sizeof(Comm::ControlBlock)
    )));
    if (!controlBlock) {
        return false;
    }
    memset(controlBlock.get(), 0, sizeof(Comm::ControlBlock));
    return true;
}

static std::string moveDllToGlobalPath() {
    std::filesystem::path path = getenv("WINDIR");
    path.append(dllName);
    if (!std::filesystem::copy_file(dllName, path,
                                    std::filesystem::copy_options::overwrite_existing)) {
        return "";
    }
    return path.string();
}

bool Draw::initialize() {
    if (initialized) {
        return true;
    }

    enableDebugPrivilege();

    // check if dll is loaded
    if (!fileMappingInitialize()) {
        std::string path = moveDllToGlobalPath();
        if (path.empty()) {
            return false;
        }
        uint32_t pid = getPidByName("dwm.exe");
        if (pid == 0) {
            return false;
        }
        if (!injectDll(pid, path)) {
            return false;
        }
        // try again
        if (!fileMappingInitialize()) {
            return false;
        }
    }
    initialized = true;
    return true;
}

static bool overflow(size_t appendSize) {
    auto arr = reinterpret_cast<Comm::DrawArray *>(controlBlock->swapchain[drawingId]);
    return drawingPtr + appendSize > reinterpret_cast<uintptr_t>(arr->buf) + Comm::BUFFER_SIZE;
}

static void increaseCount() {
    auto arr = reinterpret_cast<Comm::DrawArray *>(controlBlock->swapchain[drawingId]);
    arr->count++;
}

void Draw::begin() {
    if (!initialized) {
        return;
    }
    drawingId = (controlBlock->id + 1) % Comm::SWAPCHAIN_COUNT;
    auto arr = reinterpret_cast<Comm::DrawArray *>(controlBlock->swapchain[drawingId]);
    arr->count = 0;
    drawingPtr = reinterpret_cast<uintptr_t>(arr->buf);
}

void Draw::end() {
    if (!initialized) {
        return;
    }
    controlBlock->id = drawingId;
    controlBlock->on = true;
}

void Draw::disable() {
    if (!initialized) {
        return;
    }
    controlBlock->on = false;
}

void Draw::setClipRect(ImVec2 p1, ImVec2 p2) {
    if (!initialized) {
        return;
    }
    controlBlock->pos = p1;
    controlBlock->size = ImVec2(p2.x - p1.x, p2.y - p1.y);
}

void Draw::drawRect(
        ImVec2 p1,
        ImVec2 p2,
        ImColor col,
        float rounding,
        ImDrawFlags flags,
        float thickness
) {
    if (!initialized || overflow(sizeof(Comm::DrawRect))) {
        return;
    }
    increaseCount();
    auto *packet = reinterpret_cast<Comm::DrawRect *>(drawingPtr);
    packet->type = Comm::Type::Rect;
    packet->p1 = p1;
    packet->p2 = p2;
    packet->col = col;
    packet->rounding = rounding;
    packet->flags = flags;
    packet->thickness = thickness;
    drawingPtr += sizeof(Comm::DrawRect);
}

void Draw::drawRectFilled(
        ImVec2 p1,
        ImVec2 p2,
        ImColor col,
        float rounding,
        ImDrawFlags flags
) {
    if (!initialized || overflow(sizeof(Comm::DrawRectFilled))) {
        return;
    }
    increaseCount();
    auto *packet = reinterpret_cast<Comm::DrawRectFilled *>(drawingPtr);
    packet->type = Comm::Type::RectFilled;
    packet->p1 = p1;
    packet->p2 = p2;
    packet->col = col;
    packet->rounding = rounding;
    packet->flags = flags;
    drawingPtr += sizeof(Comm::DrawRectFilled);
}

void Draw::drawLine(
        ImVec2 p1,
        ImVec2 p2,
        ImColor col,
        float thickness
) {
    if (!initialized || overflow(sizeof(Comm::DrawLine))) {
        return;
    }
    increaseCount();
    auto *packet = reinterpret_cast<Comm::DrawLine *>(drawingPtr);
    packet->type = Comm::Type::Line;
    packet->p1 = p1;
    packet->p2 = p2;
    packet->col = col;
    packet->thickness = thickness;
    drawingPtr += sizeof(Comm::DrawLine);
}

void Draw::drawCircle(
        ImVec2 center,
        float radius,
        ImColor col,
        int segments,
        float thickness
) {
    if (!initialized || overflow(sizeof(Comm::DrawCircle))) {
        return;
    }
    increaseCount();
    auto *packet = reinterpret_cast<Comm::DrawCircle *>(drawingPtr);
    packet->type = Comm::Type::Circle;
    packet->center = center;
    packet->radius = radius;
    packet->col = col;
    packet->segments = segments;
    packet->thickness = thickness;
    drawingPtr += sizeof(Comm::DrawCircle);
}

void Draw::drawCircleFilled(
        ImVec2 center,
        float radius,
        ImColor col,
        int segments
) {
    if (!initialized || overflow(sizeof(Comm::DrawCircleFilled))) {
        return;
    }
    increaseCount();
    auto *packet = reinterpret_cast<Comm::DrawCircleFilled *>(drawingPtr);
    packet->type = Comm::Type::CircleFilled;
    packet->center = center;
    packet->radius = radius;
    packet->col = col;
    packet->segments = segments;
    drawingPtr += sizeof(Comm::DrawCircleFilled);
}

void Draw::drawString(
        ImVec2 pos,
        ImColor col,
        const std::string &str
) {
    if (!initialized || overflow(sizeof(Comm::DrawString) + str.length())) {
        return;
    }
    increaseCount();
    auto *packet = reinterpret_cast<Comm::DrawString *>(drawingPtr);
    packet->type = Comm::Type::String;
    packet->pos = pos;
    packet->col = col;
    packet->length = str.length();
    memcpy(packet->str, str.c_str(), str.length());
    drawingPtr += (sizeof(Comm::DrawString) + str.length());
}
