#ifndef ASSAULT_CUBE_HACKING_VIEW_IMGUI_D3D9_EXTERNAL_VIEW_H
#define ASSAULT_CUBE_HACKING_VIEW_IMGUI_D3D9_EXTERNAL_VIEW_H

#include "view/view.h"

class ImGuiD3D9ExternalView : public View {

    std::string targetProcess;

    std::vector<std::function<void()>> callbackList;

    void SetupWindow();

    bool DirectXInit();

    void MainLoop();

    void Render();

    void Draw();

    void InputHandler();

public:

    ImGuiD3D9ExternalView();

    static ImGuiD3D9ExternalView &getInstance();

    bool initialize(const std::string &processName) override;

    bool registerCallback(std::function<void()> callback) override;

    bool loop() override;

};

#endif //ASSAULT_CUBE_HACKING_VIEW_IMGUI_D3D9_EXTERNAL_VIEW_H
