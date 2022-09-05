#include <vector>
#include <string>
#include <imgui.h>
#include "game.h"
#include "game/interface/player_basic_interface.h"
#include "name_list.h"

NameList &NameList::getInstance() {
    static NameList instance;
    return instance;
}

void NameList::callback() {
    std::vector<std::shared_ptr<PlayerInterface>> players = Game::getInstance().getPlayers();
    for (int i = 0; i < players.size(); i++) {
        PlayerInterface &player = *players[i];
        ImGui::GetForegroundDrawList()->AddText(
                ImVec2(0, i * 10),
                ImColor(255, 255, 255),
                player.getName().c_str()
        );
    }
}

