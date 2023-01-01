#ifndef EVIL_CAT_VIEW_IMGUI_VIEW_VIEW_H
#define EVIL_CAT_VIEW_IMGUI_VIEW_VIEW_H

#include <string>
#include <chrono>
#include "view/view_interface.h"

class ImGuiDWMView : public ViewInterface {
public:
    bool initialize(uint32_t pid) override;

    bool loop() override;

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

private:
    enum { FRAME_PER_SECOND = 60 };

    uint32_t targetPid;

    bool isTargetWindowOnTop();

    std::pair<ImVec2, ImVec2> getTopWindowRect();

    void waitForNextFrame(std::chrono::time_point<std::chrono::steady_clock> start);

    void render();
};

#endif //EVIL_CAT_VIEW_IMGUI_VIEW_VIEW_H
