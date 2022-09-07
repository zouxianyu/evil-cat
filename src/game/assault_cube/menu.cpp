#include <imgui.h>
#include "service/aimbot/aimbot.h"
#include "service/esp/esp.h"
#include "service/name_list/name_list.h"
#include "service/health_locker/health_locker.h"

#include "settings.h"
#include "menu.h"

Menu &Menu::getInstance() {
    static Menu instance;
    return instance;
}

void Menu::show() {
    ImGui::Begin("Menu");
    ImGui::Text("Hacking Template");

    //show esp toggle
    ImGui::Checkbox("ESP", &Settings::Esp::on);

    ImGui::SameLine();
    ImGui::RadioButton("2D", (int *) &Settings::Esp::boxType, (int) Settings::Esp::BoxType::show2D);
    ImGui::SameLine();
    ImGui::RadioButton("3D", (int *) &Settings::Esp::boxType, (int) Settings::Esp::BoxType::show3D);

    //show aimbot
    ImGui::Checkbox("aimbot", &Settings::Aimbot::on);
    ImGui::SliderAngle("max aimbot angle", &Settings::Aimbot::maxAngle, 0.f, 180.f);

    // show name list options
    ImGui::Checkbox("name list", &Settings::NameList::on);

    // show health locker options
    ImGui::Checkbox("health locker", &Settings::HealthLocker::on);

    // show exit button
    if (ImGui::Button("exit")) {
        Settings::exit = true;
    }

    ImGui::End();
}
