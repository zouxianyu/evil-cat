#ifndef CONF_INTERNAL_COMPILE

#include <windows.h>
#include <dwmapi.h>
#include <stdio.h>
#include "entry/entry.h"

// this is the external entry
// if we compile it as an executable file
// the 'main' is the entry of the executable file
// we call that 'external entry'
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SetProcessDPIAware();
    // allocate a console for internal usage
#if (CONF_USING_STD_CONSOLE == 1)
    AllocConsole();
    FILE *f;
    freopen_s(&f, "CONOUT$", "w", stdout);
#endif
    // just call the real entry
    entry();
    // free the console
#if (CONF_USING_STD_CONSOLE == 1)
    fclose(f);
    FreeConsole();
#endif
    return 0;
}

#endif