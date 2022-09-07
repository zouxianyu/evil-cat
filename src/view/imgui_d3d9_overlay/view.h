#ifndef EVIL_CAT_VIEW_IMGUI_D3D9_OVERLAY_VIEW_H
#define EVIL_CAT_VIEW_IMGUI_D3D9_OVERLAY_VIEW_H

#include "view/view.h"

class ImGuiD3D9Overlay : public View {

public:

    static ImGuiD3D9Overlay &getInstance();

    bool initialize(const std::string &processName) override;

    bool loop() override;

};

#endif //EVIL_CAT_VIEW_IMGUI_D3D9_OVERLAY_VIEW_H
