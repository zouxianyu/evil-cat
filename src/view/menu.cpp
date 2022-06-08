#include <iostream>
#include <imgui/imgui.h>
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
    ImGui::Checkbox("ESP", &Settings::getInstance().showEsp);
    static int ESPSelection = 0;
    ImGui::SameLine();
    ImGui::RadioButton("2D", &ESPSelection, 0);
    ImGui::SameLine();
    ImGui::RadioButton("3D", &ESPSelection, 1);
    Settings::getInstance().showEspBox2D = (ESPSelection == 0);

    ImGui::Checkbox("show health", &Settings::getInstance().showHealthAndArmor);

    //show aimbot
    ImGui::Checkbox("aimbot (on shooting)", &Settings::getInstance().aimbot);
    ImGui::SliderAngle("max aimbot angle", &Settings::getInstance().aimbotMaxAngle, 0.f, 180.f);

    // show exit button
    if (ImGui::Button("exit")) {
        Settings::getInstance().exit = true;
    }

    ImGui::End();
}
