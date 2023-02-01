#ifndef EVIL_CAT_VIEW_IMGUI_D3D9_EXTERNAL_VIEW_H
#define EVIL_CAT_VIEW_IMGUI_D3D9_EXTERNAL_VIEW_H

#include <string>
#include "view/menu.h"
#include "view/view_interface.h"

class ImGuiD3D9ExternalView : public ViewInterface {

    uint32_t targetProcessPid;

    Menu menu;

    void SetupWindow();

    bool DirectXInit();

    void MainLoop();

    void Render();

    void Draw();

    void InputHandler();

    void ProcessCheck();
public:

    bool initialize(uint32_t pid) override;

    bool loop() override;

    glm::vec2 getSize() override;

    void drawRect(
            ImVec2 p1,
            ImVec2 p2,
            ImColor col,
            float rounding,
            ImDrawFlags flags,
            float thickness
    ) override;

    void drawRectFilled(
            ImVec2 p1,
            ImVec2 p2,
            ImColor col,
            float rounding,
            ImDrawFlags flags
    ) override;

    void drawLine(
            ImVec2 p1,
            ImVec2 p2,
            ImColor col,
            float thickness
    ) override;

    void drawCircle(
            ImVec2 center,
            float radius,
            ImColor col,
            int segments,
            float thickness
    ) override;

    void drawCircleFilled(
            ImVec2 center,
            float radius,
            ImColor col,
            int segments
    ) override;

    void drawString(
            ImVec2 pos,
            ImColor col,
            const std::string &str
    ) override;

};

#endif //EVIL_CAT_VIEW_IMGUI_D3D9_EXTERNAL_VIEW_H
