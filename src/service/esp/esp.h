#ifndef ASSAULT_CUBE_HACKING_SERVICE_ESP_ESP_H
#define ASSAULT_CUBE_HACKING_SERVICE_ESP_ESP_H

#include <vector>
#include <imgui.h>
#include "game/interface/player_basic_interface.h"
#include "type.h"

namespace Settings::Esp {
    extern bool on;
    enum class BoxType : int {
        show2D,
        show3D
    };
    extern BoxType boxType;
    extern bool showViewLine;
    extern bool showHeadBar;
    extern bool showHeadDot;

    extern ImColor teammateColor;
    extern ImColor enemyColor;
    extern ImColor viewLineColor;
    extern ImColor healthColor;
    extern ImColor armorColor;
}

class Esp {
    void showEsp2D(
            std::shared_ptr<PlayerBasicInterface> localPlayer,
            const std::vector<std::shared_ptr<PlayerBasicInterface>> &players
    );

//    void showEsp3D(
//            std::shared_ptr<PlayerBasicInterface> localPlayer,
//            std::vector<std::shared_ptr<PlayerBasicInterface>> players
//    );
//
//    void showViewLine(
//            std::shared_ptr<PlayerBasicInterface> localPlayer,
//            std::vector<std::shared_ptr<PlayerBasicInterface>> players
//    );
//
//    void showHeadBar(
//            std::shared_ptr<PlayerBasicInterface> localPlayer,
//            std::vector<std::shared_ptr<PlayerBasicInterface>> players
//    );
//
//    void showHeadDot(
//            std::shared_ptr<PlayerBasicInterface> localPlayer,
//            std::vector<std::shared_ptr<PlayerBasicInterface>> players
//    );

public:
    static Esp &getInstance();

    void callback();
};

#endif //ASSAULT_CUBE_HACKING_SERVICE_ESP_ESP_H
