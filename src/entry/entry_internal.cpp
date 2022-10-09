#ifdef CONF_INTERNAL_COMPILE

#include <thread>
#include <windows.h>
#include "entry/entry.h"

// if we inject the dll into the target process
// and when we want to exit, we need to free the library
static void entryWrapper(HMODULE hModule) {

    // allocate a console for internal usage
#if (CONF_USING_STD_CONSOLE == 1)
    AllocConsole();
    FILE *f;
    freopen_s(&f, "CONOUT$", "w", stdout);
#endif

    // call the real entry
    entry();

    // free the console
#if (CONF_USING_STD_CONSOLE == 1)
    fclose(f);
    FreeConsole();
#endif

    // free the library
    // so we can inject the dll to the target process again
    FreeLibraryAndExitThread(hModule, 0);
}

// DllMain is the entry point of the dll
// we call that 'internal entry'
BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
) {
    // if the dll is injected into the target process
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {

        // create a thread and detach from it
        // so the DllMain can return immediately
        std::thread t(entryWrapper, hModule);
        t.detach();
    }
    return TRUE;
}

#endif