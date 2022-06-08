#include <windows.h>
#include <imgui.h>
#include "view.h"

ImGuiD3D9Overlay &ImGuiD3D9Overlay::getInstance() {
    static ImGuiD3D9Overlay instance;
    return instance;
}

bool ImGuiD3D9Overlay::initialize(const std::string &processName) {
    return false;
}

bool ImGuiD3D9Overlay::loop() {
    return false;
}

