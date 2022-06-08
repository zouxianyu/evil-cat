#include <iostream>
#include <iomanip>
#include <memory>
#include <thread>
#include <Windows.h>
#include <imgui/imgui.h>
#include <glm/ext/scalar_constants.hpp>
#include "entity/entity_manager.h"
#include "mem/process_memory.h"
#include "mem/buffer_pool.h"
#include "mem/process_memory_accessor.h"
#include "proc/process_info.h"
#include "view/view.h"
#include "service/world_to_screen/world_to_screen.h"
#include "service/world_to_screen/self_matrix_world_to_screen.h"
#include "service/esp/esp.h"
#include "service/aimbot/aimbot.h"
#include "entry.h"



void entry() {

    ProcessInfo::getInstance().attach(CONF_PROCESS_NAME);
    ProcessMemory::getInstance().attach(CONF_PROCESS_NAME);

    View::getInstance().initialize(CONF_PROCESS_NAME);

    View::getInstance().registerCallback([]() {
        std::vector<Player> players = EntityManager::getInstance().getPlayers();
        for (int i = 0; i < players.size(); i++) {
            Player &player = players[i];
            ImGui::GetForegroundDrawList()->AddText(
                    ImVec2(0, i*10), ImColor(255, 255, 255), player.getName().c_str()
            );
        }
    });

    View::getInstance().registerCallback(
            std::bind(&Esp::renderCallback, &Esp::getInstance())
    );
    View::getInstance().registerCallback(
            std::bind(&Aimbot::callback, &Aimbot::getInstance())
    );

    View::getInstance().registerCallback(
            std::bind(&BufferPool::refresh, &BufferPool::getInstance())
    );

    View::getInstance().loop();

//    while (!(GetAsyncKeyState(VK_RSHIFT) & 1)) {
//        ProcessMemory::getInstance().refresh();
//        Player localPlayer = EntityManager::getInstance().getLocalPlayer();
//
//        std::cout << "player " << localPlayer.getName()
//                  << " health: " << std::setw(3) << localPlayer.getHealth() << std::endl;
//        Vec3 head = localPlayer.getHead();
//        Vec3 feet = localPlayer.getFeet();
//        Vec3 viewAngle = localPlayer.getViewAngle();
//        std::cout << " head: " << head.x << " " << head.y << " " << head.z
//                  << " feet: " << feet.x << " " << feet.y << " " << feet.z
//                  << " viewAngle: " << viewAngle.x << " " << viewAngle.y << " " << viewAngle.z << std::endl;
//
//        WorldToScreen::getInstance().refresh();
//        Vec2 screen{};
//        if (WorldToScreen::getInstance().translate(localPlayer.getFeet(), screen)) {
//            std::cout <<"screen: " << screen.x << " " << screen.y << std::endl;
//        }


//        std::vector<Player> players = EntityManager::getInstance().getPlayers();
//        for (int i = 0; i < players.size(); i++) {
//            Player &player = players[i];
////            Vec2 screen{};
////            if (!WorldToScreen::getInstance().translate(player.getHead(), screen)) {
////                continue;
////            }
////            std::cout << "player " << i << ": " << player.getName() << "\t" << screen.x << " " << screen.y << std::endl;
//        }
//        std::cout << std::endl;
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    }

    ProcessMemory::getInstance().detach();
    ProcessInfo::getInstance().detach();

}

