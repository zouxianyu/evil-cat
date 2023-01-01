#ifndef DWM_OVERLAY_COMM_PACKET_H
#define DWM_OVERLAY_COMM_PACKET_H

#include <cstdint>
#include <type_traits>
#include <array>
#include <functional>
#include <imgui.h>

namespace Comm {
    const char *const FILE_MAPPING_NAME = "dwm-draw-buffer";

    enum {
        SWAPCHAIN_COUNT = 2,
        BUFFER_SIZE = 0x1000 * 10
    };

    struct ControlBlock {
        bool on;
        uint8_t id; // 0 - 1
        ImVec2 pos;
        ImVec2 size;
        uint8_t swapchain[SWAPCHAIN_COUNT][BUFFER_SIZE];
    };

    struct DrawArray {
        uint32_t count;
        uint8_t buf[0];
    };

    enum class Type : uint8_t {
        Rect,
        RectFilled,
        Line,
        Circle,
        CircleFilled,
        String,
    };

    struct DrawHeader {
        Type type;
    };

    struct DrawRect : DrawHeader {
        ImVec2 p1;
        ImVec2 p2;
        ImColor col;
        float rounding;
        ImDrawFlags flags;
        float thickness;
    };

    struct DrawRectFilled : DrawHeader {
        ImVec2 p1;
        ImVec2 p2;
        ImColor col;
        float rounding;
        ImDrawFlags flags;
    };

    struct DrawLine : DrawHeader {
        ImVec2 p1;
        ImVec2 p2;
        ImColor col;
        float thickness;
    };

    struct DrawCircle : DrawHeader {
        ImVec2 center;
        float radius;
        ImColor col;
        int segments;
        float thickness;
    };

    struct DrawCircleFilled : DrawHeader {
        ImVec2 center;
        float radius;
        ImColor col;
        int segments;
    };

    struct DrawString : DrawHeader {
        ImVec2 pos;
        ImColor col;
        uint32_t length;
        char str[0];
    };

    const static std::function<size_t(DrawHeader *)> sizeTable[] = {
            [](DrawHeader *) -> size_t {
                return sizeof(DrawRect);
            },
            [](DrawHeader *) -> size_t {
                return sizeof(DrawRectFilled);
            },
            [](DrawHeader *) -> size_t {
                return sizeof(DrawLine);
            },
            [](DrawHeader *) -> size_t {
                return sizeof(DrawCircle);
            },
            [](DrawHeader *) -> size_t {
                return sizeof(DrawCircleFilled);
            },
            [](DrawHeader *packet) -> size_t {
                auto ptr = reinterpret_cast<DrawString *>(packet);
                return sizeof(DrawString) + ptr->length;
            },
    };

}

#endif //DWM_OVERLAY_COMM_PACKET_H
