#ifndef EVIL_CAT_SERVICE_ESP_ESP_H
#define EVIL_CAT_SERVICE_ESP_ESP_H

#include <vector>
#include <imgui.h>
#include "game/interface/player_basic_interface.h"

namespace Settings::Esp {
    extern bool on;
    enum class BoxType : int {
        show2D,
        show3D
    };
    extern BoxType boxType;
    extern bool showViewLine;
    extern bool showHeadBar;
    extern bool showHeadCircle;
    extern bool showDistance;

    extern ImColor teammateColor;
    extern ImColor enemyColor;

    extern float barMoveUp;
    extern float barWidth;
    extern float barHeight;

    extern ImColor healthFullColor;
    extern ImColor healthHalfColor;
    extern ImColor healthLowColor;
}

class Esp {
    void showEsp2D(
            std::shared_ptr<PlayerBasicInterface> localPlayer,
            const std::vector<std::shared_ptr<PlayerBasicInterface>> &players
    );

    void showEsp3D(
            std::shared_ptr<PlayerBasicInterface> localPlayer,
            std::vector<std::shared_ptr<PlayerBasicInterface>> players
    );

    void showViewLine(
            std::shared_ptr<PlayerBasicInterface> localPlayer,
            std::vector<std::shared_ptr<PlayerBasicInterface>> players
    );

    void showHeadBar(
            std::shared_ptr<PlayerBasicInterface> localPlayer,
            std::vector<std::shared_ptr<PlayerBasicInterface>> players
    );

    void showHeadCircle(
            std::shared_ptr<PlayerBasicInterface> localPlayer,
            std::vector<std::shared_ptr<PlayerBasicInterface>> players
    );

    void showDistance(
            std::shared_ptr<PlayerBasicInterface> localPlayer,
            std::vector<std::shared_ptr<PlayerBasicInterface>> players
    );

public:
    static Esp &getInstance();

    void callback();
};

#endif //EVIL_CAT_SERVICE_ESP_ESP_H
