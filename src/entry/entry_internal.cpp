#include <thread>
#include <windows.h>
#include "service/entry.h"

// alloc console for injected dll if needed
static void entryWrapper(HMODULE hModule) {
#if (CONF_USING_STD_CONSOLE == 1)
    AllocConsole();
    FILE *f;
    freopen_s(&f, "CONOUT$", "w", stdout);
#endif

    entry();

#if (CONF_USING_STD_CONSOLE == 1)
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
#endif
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        std::thread t(entryWrapper, hModule);
        t.detach();
    }
    return TRUE;
}