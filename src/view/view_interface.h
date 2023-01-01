#ifndef EVIL_CAT_VIEW_VIEW_INTERFACE_H
#define EVIL_CAT_VIEW_VIEW_INTERFACE_H

#include <string>
#include <functional>
#include <imgui.h>

class ViewInterface {
public:
    virtual bool initialize(const std::string &processName) = 0;

    virtual bool loop() = 0;

    virtual void drawRect(
            ImVec2 p1,
            ImVec2 p2,
            ImColor col,
            float rounding = 0.0f,
            ImDrawFlags flags = 0,
            float thickness = 1.0f
    ) = 0;

    virtual void drawRectFilled(
            ImVec2 p1,
            ImVec2 p2,
            ImColor col,
            float rounding = 0.0f,
            ImDrawFlags flags = 0
    ) = 0;

    virtual void drawLine(
            ImVec2 p1,
            ImVec2 p2,
            ImColor col,
            float thickness = 1.0f
    ) = 0;

    virtual void drawCircle(
            ImVec2 center,
            float radius,
            ImColor col,
            int segments = 0,
            float thickness = 1.0f
    ) = 0;

    virtual void drawCircleFilled(
            ImVec2 center,
            float radius,
            ImColor col,
            int segments = 0
    ) = 0;

    virtual void drawString(
            ImVec2 pos,
            ImColor col,
            const std::string &str
    ) = 0;

    virtual ~ViewInterface() = default;
};

#endif //EVIL_CAT_VIEW_VIEW_INTERFACE_H
