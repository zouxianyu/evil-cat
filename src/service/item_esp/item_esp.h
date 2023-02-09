#ifndef EVIL_CAT_ITEM_ESP_H
#define EVIL_CAT_ITEM_ESP_H

#include <vector>
#include <string>
#include <imgui.h>
#include "game/item_interface.h"
#include "service/service_interface.h"

namespace Settings::ItemEsp {
    extern bool on;
    extern int level;
    extern bool sortByDistance;

    extern float barMoveUp;
    extern float barWidth;
    extern float barHeight;
    extern float barTextShift;

    extern ImColor textColor;
    extern ImColor backgroundColor;
    extern ImColor borderColor;

    extern bool colorGradient;
    extern float noGradientAlpha;
    extern std::pair<float, float> alphaMax;
    extern std::pair<float, float> alphaMin;
}

class ItemEsp : public ServiceInterface {
public:
    std::string getName() override;

    void menuCallback() override;

    void serviceCallback() override;

private:
    void sortItemsByDistance(
            const std::shared_ptr<PlayerInterface>& localPlayer,
            std::vector<std::shared_ptr<ItemInterface>>& items
    );

    void showItems(
            const std::shared_ptr<PlayerInterface>& localPlayer,
            const std::vector<std::shared_ptr<ItemInterface>>& items
    );

    ImColor getColorWithAlpha(ImColor basicColor, float distance);
};

#endif //EVIL_CAT_ITEM_ESP_H
