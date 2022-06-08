#include "view.h"

ImGuiD3D9ExternalOfficialView &ImGuiD3D9ExternalOfficialView::getInstance() {
    static ImGuiD3D9ExternalOfficialView instance;
    return instance;
}

bool ImGuiD3D9ExternalOfficialView::initialize(const std::string &processName) {
    return false;
}

bool ImGuiD3D9ExternalOfficialView::loop() {
    return false;
}

