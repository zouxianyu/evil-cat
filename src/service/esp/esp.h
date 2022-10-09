#ifndef EVIL_CAT_SERVICE_ESP_ESP_H
#define EVIL_CAT_SERVICE_ESP_ESP_H

#include <vector>
#include <imgui.h>
#include "service/service_interface.h"
#include "module.h"

namespace Settings::Esp {
    extern bool on;
    enum class BoxType : int {
        show2D,
        show3D
    };
    extern BoxType boxType;
    extern bool showBone;
    extern bool showViewLine;
    extern bool showHeadBar;
    extern bool showHeadCircle;
    extern bool showDistance;

    extern ImColor teammateColor;
    extern ImColor enemyColor;
    extern ImColor boneColor;

    extern float barMoveUp;
    extern float barWidth;
    extern float barHeight;

    extern ImColor healthFullColor;
    extern ImColor healthHalfColor;
    extern ImColor healthLowColor;
}

class Esp : public ServiceInterface {
    void showEsp2D(
            const std::shared_ptr<PlayerInterface>& localPlayer,
            const std::vector<std::shared_ptr<PlayerInterface>> &players
    );

    void showEsp3D(
            const std::shared_ptr<PlayerInterface>& localPlayer,
            const std::vector<std::shared_ptr<PlayerInterface>>& players
    );

    void showViewLine(
            const std::shared_ptr<PlayerInterface>& localPlayer,
            const std::vector<std::shared_ptr<PlayerInterface>>& players
    );

    void showHeadBar(
            const std::shared_ptr<PlayerInterface>& localPlayer,
            const std::vector<std::shared_ptr<PlayerInterface>>& players
    );

    void showHeadCircle(
            const std::shared_ptr<PlayerInterface>& localPlayer,
            const std::vector<std::shared_ptr<PlayerInterface>>& players
    );

    void showDistance(
            const std::shared_ptr<PlayerInterface>& localPlayer,
            const std::vector<std::shared_ptr<PlayerInterface>>& players
    );

    void showBone(
            const std::shared_ptr<PlayerInterface>& localPlayer,
            const std::vector<std::shared_ptr<PlayerInterface>>& players
    );

public:
    void callback() override;

};

#endif //EVIL_CAT_SERVICE_ESP_ESP_H
