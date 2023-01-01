#include <vector>
#include <string>
#include <imgui.h>
#include "module.h"
#include "name_list.h"

namespace Settings::NameList {
    bool on = false;
}

std::string NameList::getName() {
    return "name list";
}

void NameList::menuCallback() {
    ImGui::Checkbox("name list", &Settings::NameList::on);
}

void NameList::serviceCallback() {

    if (!Settings::NameList::on) {
        return;
    }

    std::vector<std::shared_ptr<PlayerInterface>> players = Module::game->getPlayers();

    for (int i = 0; i < players.size(); i++) {
        PlayerInterface &player = *players[i];
        Module::view->drawString(
                ImVec2(0, i * ImGui::GetTextLineHeightWithSpacing()),
                ImColor(255, 255, 255),
                player.getName()
        );
    }
}

