#include <vector>
#include <memory>
#include <algorithm>
#include "game/item_interface.h"
#include "world_to_screen/world_to_screen.h"
#include "module.h"
#include "settings.h"
#include "item_esp.h"

namespace Settings::ItemEsp {
    bool on = true;
    int level = 0;
    bool sortByDistance = true;

    float barMoveUp = 10.f;
    float barWidth = 60.f;
    float barHeight = 20.f;
    float barTextShift = 2.f;

    ImColor textColor = ImColor(255, 255, 255);
    ImColor backgroundColor = ImColor(50, 50, 50);
    ImColor borderColor = ImColor(255, 255, 255);

    bool colorGradient = true;
    float noGradientAlpha = 200.f / 255.f;
    // (distance, alpha)
    std::pair<float, float> alphaMax = {0.f, 255.f / 255.f};
    std::pair<float, float> alphaMin = {100.f, 150.f / 255.f};
}

std::string ItemEsp::getName() {
    return "Item ESP";
}

void ItemEsp::menuCallback() {

}

void ItemEsp::serviceCallback() {
    if (!Settings::ItemEsp::on) {
        return;
    }

    EntityContainer container = Module::game->getEntities();
    std::shared_ptr<PlayerInterface> &localPlayer = container.localPlayer;
    std::vector<std::shared_ptr<ItemInterface>> &items = container.items;

    if (Settings::ItemEsp::sortByDistance) {
        sortItemsByDistance(localPlayer, items);
    }

    showItems(localPlayer, items);
}

void ItemEsp::sortItemsByDistance(
        const std::shared_ptr<PlayerInterface> &localPlayer,
        std::vector<std::shared_ptr<ItemInterface>> &items
) {
    glm::vec3 position = localPlayer->getPosition();

    std::sort(items.begin(), items.end(),
              [position](const auto &a, const auto &b) {
                  return glm::distance(a->getPosition(), position) >
                         glm::distance(b->getPosition(), position);
              });
}

void ItemEsp::showItems(
        const std::shared_ptr<PlayerInterface> &localPlayer,
        const std::vector<std::shared_ptr<ItemInterface>> &items
) {
    glm::vec3 localPosition = localPlayer->getPosition();

    for (const auto &item: items) {
        if (item->getLevel() < Settings::ItemEsp::level) {
            continue;
        }

        glm::vec3 position = item->getPosition();
        std::optional<glm::vec2> screenPos = WorldToScreen::translate(position);
        if (!screenPos) {
            continue;
        }

        float distance = glm::distance(position, localPosition) * Settings::distanceFactor;
        const ImColor textColor =
                getColorWithAlpha(Settings::ItemEsp::textColor, distance);
        const ImColor backgroundColor =
                getColorWithAlpha(Settings::ItemEsp::backgroundColor, distance);
        const ImColor borderColor =
                getColorWithAlpha(Settings::ItemEsp::borderColor, distance);

        float realBarHeight = item->getExtra().empty() ?
                              Settings::ItemEsp::barHeight :
                              Settings::ItemEsp::barHeight * 2;

        glm::vec2 barLeftTop = {
                screenPos->x - Settings::ItemEsp::barWidth / 2,
                screenPos->y - (realBarHeight + Settings::ItemEsp::barMoveUp)
        };
        glm::vec2 barLeftBottom = {
                screenPos->x - Settings::ItemEsp::barWidth / 2,
                screenPos->y - Settings::ItemEsp::barMoveUp
        };
        glm::vec2 barRightBottom = {
                screenPos->x + Settings::ItemEsp::barWidth / 2,
                screenPos->y - Settings::ItemEsp::barMoveUp
        };

        Module::view->drawRectFilled(
                ImVec2(barLeftTop.x, barLeftTop.y),
                ImVec2(barRightBottom.x, barRightBottom.y),
                backgroundColor
        );
        Module::view->drawRect(
                ImVec2(barLeftTop.x, barLeftTop.y),
                ImVec2(barRightBottom.x, barRightBottom.y),
                borderColor
        );
        Module::view->drawLine(
                ImVec2(screenPos->x, screenPos->y),
                ImVec2(barLeftBottom.x, barLeftBottom.y),
                borderColor
        );
        Module::view->drawLine(
                ImVec2(screenPos->x, screenPos->y),
                ImVec2(barRightBottom.x, barRightBottom.y),
                borderColor
        );

        Module::view->drawString(
                ImVec2(barLeftTop.x + Settings::ItemEsp::barTextShift, barLeftTop.y),
                textColor,
                item->getName()
        );

        if (!item->getExtra().empty()) {
            Module::view->drawString(
                    ImVec2(barLeftTop.x + Settings::ItemEsp::barTextShift,
                           barLeftTop.y + Settings::ItemEsp::barHeight),
                    textColor,
                    item->getExtra()
            );
        }
    }
}

ImColor ItemEsp::getColorWithAlpha(ImColor basicColor, float distance) {
    if (!Settings::ItemEsp::colorGradient) {
        return {basicColor.Value.x, basicColor.Value.y, basicColor.Value.z,
                Settings::ItemEsp::noGradientAlpha};

    } else {
        auto [x0, y0] = Settings::ItemEsp::alphaMax;
        auto [x1, y1] = Settings::ItemEsp::alphaMin;

        // linear interpolation
        float alpha = (y1 - y0) / (x1 - x0) * (distance - x0) + y0;

        // alpha has to be in range [y1, y0]
        alpha = std::clamp(alpha, y1, y0);

        return {basicColor.Value.x, basicColor.Value.y, basicColor.Value.z, alpha};
    }
}
