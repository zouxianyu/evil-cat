#ifndef EVIL_CAT_VIEW_IMGUI_D3D9_EXTERNAL_VIEW_H
#define EVIL_CAT_VIEW_IMGUI_D3D9_EXTERNAL_VIEW_H

#include <string>
#include "view/view_interface.h"

class ImGuiD3D9ExternalView : public ViewInterface {

    std::string targetProcess;

    void SetupWindow();

    bool DirectXInit();

    void MainLoop();

    void Render();

    void Draw();

    void InputHandler();

    void ProcessCheck();
public:

    ImGuiD3D9ExternalView();

    static ImGuiD3D9ExternalView &getInstance();

    bool initialize(const std::string &processName) override;

    bool loop() override;

};

#endif //EVIL_CAT_VIEW_IMGUI_D3D9_EXTERNAL_VIEW_H
