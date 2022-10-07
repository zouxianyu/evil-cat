#ifndef EVIL_CAT_VIEW_IMGUI_D3D9_OVERLAY_VIEW_H
#define EVIL_CAT_VIEW_IMGUI_D3D9_OVERLAY_VIEW_H

#include "view/view_interface.h"

class ImGuiD3D9Overlay : public ViewInterface {
public:
    bool initialize(const std::string &processName) override;

    bool loop() override;

};

#endif //EVIL_CAT_VIEW_IMGUI_D3D9_OVERLAY_VIEW_H
