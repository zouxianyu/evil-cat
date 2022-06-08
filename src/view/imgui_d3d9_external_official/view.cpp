#include "view.h"

ImGuiD3D9ExternalOfficialView &ImGuiD3D9ExternalOfficialView::getInstance() {
    static ImGuiD3D9ExternalOfficialView instance;
    return instance;
}

