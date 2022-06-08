#include <iostream>
#include <iomanip>
#include <memory>
#include <thread>
#include <imgui/imgui.h>
#include <glm/ext/scalar_constants.hpp>
#include "entity/entity_manager.h"
#include "mem/process_memory.h"
#include "mem/buffer_pool.h"
#include "proc/process_info.h"
#include "view/view.h"
#include "service/esp/esp.h"
#include "service/aimbot/aimbot.h"
#include "controller/controller.h"
#include "settings.h"
#include "entry.h"

void entry() {

    ProcessInfo::getInstance().attach(CONF_PROCESS_NAME);
    ProcessMemory::getInstance().attach(CONF_PROCESS_NAME);

    View::getInstance().initialize(CONF_PROCESS_NAME);

    Controller::getInstance().registerCallback(ON_ALWAYS,[]() {
        std::vector<Player> players = EntityManager::getInstance().getPlayers();
        for (int i = 0; i < players.size(); i++) {
            Player &player = players[i];
            ImGui::GetForegroundDrawList()->AddText(
                    ImVec2(0, i*10), ImColor(255, 255, 255), player.getName().c_str()
            );
        }
    });

    Controller::getInstance().registerCallback(ON_CONDITION(Settings::getInstance().showEsp),
            std::bind(&Esp::renderCallback, &Esp::getInstance())
    );
    Controller::getInstance().registerCallback(ON_CONDITION(Settings::getInstance().aimbot),
            std::bind(&Aimbot::callback, &Aimbot::getInstance())
    );

    Controller::getInstance().registerCallback(ON_ALWAYS,
            std::bind(&BufferPool::refresh, &BufferPool::getInstance())
    );

    View::getInstance().loop();

    ProcessMemory::getInstance().detach();
    ProcessInfo::getInstance().detach();

}

