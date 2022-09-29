#include <vector>
#include <string>
#include <imgui.h>
#include "game.h"
#include "game/interface/player_basic_interface.h"
#include "game/game_helper.h"
#include "name_list.h"

namespace Settings::NameList {
    bool on = false;
}

void NameList::callback() {

    if (!Settings::NameList::on) {
        return;
    }

    std::vector<std::shared_ptr<PlayerBasicInterface>> players =
            GameHelper::getPlayers<PlayerBasicInterface>();

    for (int i = 0; i < players.size(); i++) {
        PlayerBasicInterface &player = *players[i];
        ImGui::GetBackgroundDrawList()->AddText(
                ImVec2(0, i * ImGui::GetTextLineHeightWithSpacing()),
                ImColor(255, 255, 255),
                player.getName().c_str()
        );
    }
}

