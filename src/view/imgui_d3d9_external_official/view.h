#ifndef ASSAULT_CUBE_HACKING_VIEW_IMGUI_D3D9_EXTERNAL_OFFICIAL_VIEW_H
#define ASSAULT_CUBE_HACKING_VIEW_IMGUI_D3D9_EXTERNAL_OFFICIAL_VIEW_H

#include "view/view.h"

class ImGuiD3D9ExternalOfficialView : public View {

public:

    static ImGuiD3D9ExternalOfficialView &getInstance();

    bool initialize(const std::string &processName) override;

    bool registerCallback(std::function<void()> callback) override;

    bool loop() override;

};

#endif //ASSAULT_CUBE_HACKING_VIEW_IMGUI_D3D9_EXTERNAL_OFFICIAL_VIEW_H
