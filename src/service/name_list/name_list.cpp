#include <vector>
#include <string>
#include <imgui.h>
#include "module.h"
#include "name_list.h"

namespace Settings::NameList {
    bool on = false;
}

void NameList::callback() {

    if (!Settings::NameList::on) {
        return;
    }

    std::vector<std::shared_ptr<PlayerInterface>> players = Module::game->getPlayers();

    for (int i = 0; i < players.size(); i++) {
        PlayerInterface &player = *players[i];
        ImGui::GetBackgroundDrawList()->AddText(
                ImVec2(0, i * ImGui::GetTextLineHeightWithSpacing()),
                ImColor(255, 255, 255),
                player.getName().c_str()
        );
    }
}

