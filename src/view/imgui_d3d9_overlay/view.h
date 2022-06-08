#ifndef ASSAULT_CUBE_HACKING_VIEW_IMGUI_D3D9_OVERLAY_H
#define ASSAULT_CUBE_HACKING_VIEW_IMGUI_D3D9_OVERLAY_H

#include "view/view.h"

class ImGuiD3D9Overlay : public View {

public:

    static ImGuiD3D9Overlay &getInstance();

    bool initialize(const std::string &processName) override;

    bool loop() override;

};

#endif //ASSAULT_CUBE_HACKING_VIEW_IMGUI_D3D9_OVERLAY_H
