#ifndef CONF_INTERNAL_COMPILE

#include <dwmapi.h>
#include "entry/entry.h"

// this is the external entry
// if we compile it as an executable file
// the 'main' is the entry of the executable file
// we call that 'external entry'
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SetProcessDPIAware();
    // just call the real entry
    entry();
    return 0;
}

#endif