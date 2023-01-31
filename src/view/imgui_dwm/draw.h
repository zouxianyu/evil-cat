#ifndef DWM_OVERLAY_COMM_DRAW_H
#define DWM_OVERLAY_COMM_DRAW_H

#include <string>
#include <imgui.h>

namespace Draw {
    bool initialize();

    void begin();

    void end();

    void disable();

    void setClipRect(ImVec2 p1, ImVec2 p2);

    void drawRect(
            ImVec2 p1,
            ImVec2 p2,
            ImColor col,
            float rounding = 0.0f,
            ImDrawFlags flags = 0,
            float thickness = 1.0f
    );

    void drawRectFilled(
            ImVec2 p1,
            ImVec2 p2,
            ImColor col,
            float rounding = 0.0f,
            ImDrawFlags flags = 0
    );

    void drawLine(
            ImVec2 p1,
            ImVec2 p2,
            ImColor col,
            float thickness = 1.0f
    );

    void drawCircle(
            ImVec2 center,
            float radius,
            ImColor col,
            int segments = 0,
            float thickness = 1.0f
    );

    void drawCircleFilled(
            ImVec2 center,
            float radius,
            ImColor col,
            int segments = 0
    );

    void drawString(
            ImVec2 pos,
            ImColor col,
            const std::string &str
    );
}

#endif //DWM_OVERLAY_COMM_DRAW_H
