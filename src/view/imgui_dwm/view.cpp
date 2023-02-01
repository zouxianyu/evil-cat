#include <windows.h>
#include "proc/process_helper.h"
#include "controller/controller.h"
#include "settings.h"
#include "draw.h"
#include "view.h"

bool ImGuiDWMView::initialize(uint32_t pid) {
    if (!pid) {
        return false;
    }

    targetPid = pid;

    return Draw::initialize();
}

bool ImGuiDWMView::loop() {
    while(!Settings::exit) {
        auto start = std::chrono::steady_clock::now();

        if (!ProcessHelper::isAlive(targetPid)) {
            Draw::disable();
            Settings::exit = true;
            break;
        }

        if (!isTargetWindowOnTop()) {
            Draw::disable();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        render();

        waitForNextFrame(start);
    }
    return true;
}

glm::vec2 ImGuiDWMView::getSize() {
    return {currentWindowRect.second.x - currentWindowRect.first.x,
            currentWindowRect.second.y - currentWindowRect.first.y};
}

bool ImGuiDWMView::isTargetWindowOnTop() {
    DWORD topPid = 0;
    GetWindowThreadProcessId(GetForegroundWindow(), &topPid);
    return topPid == targetPid;
}

std::pair<ImVec2, ImVec2> ImGuiDWMView::getTopWindowRect() {
    HWND hwnd = GetForegroundWindow();
    RECT rect;
    GetClientRect(hwnd, &rect);
    POINT p1 = { rect.left, rect.top };
    POINT p2 = { rect.right, rect.bottom };
    ClientToScreen(hwnd, &p1);
    ClientToScreen(hwnd, &p2);

    return {ImVec2(p1.x, p1.y), ImVec2(p2.x, p2.y)};
}

void ImGuiDWMView::waitForNextFrame(std::chrono::time_point<std::chrono::steady_clock> start) {
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    auto sleepTime = std::chrono::milliseconds(1000 / FRAME_PER_SECOND) - duration;
    if (sleepTime.count() > 0) {
        std::this_thread::sleep_for(sleepTime);
    }
}

void ImGuiDWMView::render() {
    currentWindowRect = getTopWindowRect();
    Draw::setClipRect(currentWindowRect.first, currentWindowRect.second);

    Draw::begin();
    for (auto &callback : Controller::getInstance().getServiceCallbacks()) {
        callback();
    }
    Draw::end();
}

void ImGuiDWMView::drawRect(
        ImVec2 p1,
        ImVec2 p2,
        ImColor col,
        float rounding,
        ImDrawFlags flags,
        float thickness
) {
    Draw::drawRect(p1, p2, col, rounding, flags, thickness);
}

void ImGuiDWMView::drawRectFilled(
        ImVec2 p1,
        ImVec2 p2,
        ImColor col,
        float rounding,
        ImDrawFlags flags
) {
    Draw::drawRectFilled(p1, p2, col, rounding, flags);
}

void ImGuiDWMView::drawLine(
        ImVec2 p1,
        ImVec2 p2,
        ImColor col,
        float thickness
) {
    Draw::drawLine(p1, p2, col, thickness);
}

void ImGuiDWMView::drawCircle(
        ImVec2 center,
        float radius,
        ImColor col,
        int segments,
        float thickness
) {
    Draw::drawCircle(center, radius, col, segments, thickness);
}

void ImGuiDWMView::drawCircleFilled(
        ImVec2 center,
        float radius,
        ImColor col,
        int segments
) {
    Draw::drawCircleFilled(center, radius, col, segments);
}

void ImGuiDWMView::drawString(
        ImVec2 pos,
        ImColor col,
        const std::string &str
) {
    Draw::drawString(pos, col, str);
}
