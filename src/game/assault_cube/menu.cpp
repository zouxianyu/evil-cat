#include <imgui.h>
#include "service/aimbot/aimbot.h"
#include "service/esp/esp.h"
#include "service/radar/radar.h"
#include "service/name_list/name_list.h"
#include "service/health_locker/health_locker.h"

#include "settings.h"
#include "menu.h"

Menu &Menu::getInstance() {
    static Menu instance;
    return instance;
}

void Menu::show() {
    constexpr auto windowFlags = ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoMove;
    constexpr float tabWidth = 120.f;
    constexpr float windowWidth = 600.f;
    constexpr float windowHeight = 400.f;

    enum class Tab {
        Info,
        Basic,
        Extra,
        Color,
        Exit,
        End,
    };
    static Tab tab = Tab::Info;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.f);
    ImGui::Begin("Evil Cat", &Settings::showMenu, windowFlags);
    ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight));
    ImVec2 globalRegion = ImGui::GetContentRegionAvail();

    // create a widget at left
    ImGui::BeginChild(
            "tab panel",
            ImVec2(tabWidth, globalRegion.y),
            false
    );
    ImVec2 panelRegion = ImGui::GetContentRegionAvail();
    float tabHeight = panelRegion.y / static_cast<int>(Tab::End) - 3.f;
    if (ImGui::Button(
            "Info",
            ImVec2(ImGui::GetContentRegionAvail().x, tabHeight)
    )) {
        tab = Tab::Info;
    }
    if (ImGui::Button(
            "Basic",
            ImVec2(ImGui::GetContentRegionAvail().x, tabHeight)
    )) {
        tab = Tab::Basic;
    }
    if (ImGui::Button(
            "Extra",
            ImVec2(ImGui::GetContentRegionAvail().x, tabHeight)
    )) {
        tab = Tab::Extra;
    }
    if (ImGui::Button(
            "Color",
            ImVec2(ImGui::GetContentRegionAvail().x, tabHeight)
    )) {
        tab = Tab::Color;
    }
    if (ImGui::Button(
            "Exit",
            ImVec2(ImGui::GetContentRegionAvail().x, tabHeight)
    )) {
        ImGui::OpenPopup("Exit");
    }

    if (ImGui::BeginPopupModal("Exit", NULL, windowFlags))
    {
        ImGui::Text("Are you sure you want to exit?");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            Settings::exit = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild(
            "tab content",
            ImVec2(globalRegion.x - tabWidth, globalRegion.y),
            false
    );
    const char *boxType[] = { "2D", "3D" };
    const char *aimbotStrategy[] = {
            "right button precise",
            "right button smooth",
            "auto follow precise",
            "auto follow smooth",
    };
    switch (tab) {
        case Tab::Info:
            ImGui::Text("Info");
            ImGui::Separator();

            ImGui::Text("Welcome to use the FPS hacking framework: Evil Cat!");
            ImGui::Text("Press right shift to hide/open the menu.");
            break;
        case Tab::Basic:
            ImGui::Text("Basic");
            ImGui::Separator();

            ImGui::Checkbox("ESP", &Settings::Esp::on);
            // make the item as small as possible
            ImGui::PushItemWidth(70.f);
            ImGui::Combo(
                    "box type",
                    (int *) &Settings::Esp::boxType,
                    boxType,
                    IM_ARRAYSIZE(boxType)
            );
            ImGui::PopItemWidth();
            ImGui::Checkbox("view line", &Settings::Esp::showViewLine);
            ImGui::Checkbox("head bar", &Settings::Esp::showHeadBar);
            ImGui::Checkbox("head circle", &Settings::Esp::showHeadCircle);
            ImGui::Checkbox("distance", &Settings::Esp::showDistance);
            ImGui::Separator();

            ImGui::Checkbox("aimbot", &Settings::Aimbot::on);
            ImGui::Combo(
                    "strategy",
                    (int *) &Settings::Aimbot::strategy,
                    aimbotStrategy,
                    IM_ARRAYSIZE(aimbotStrategy)
            );
            ImGui::SliderAngle(
                    "max angle",
                    &Settings::Aimbot::maxAngle,
                    0.f,
                    180.f
            );
            ImGui::Separator();

            ImGui::Checkbox("2D radar", &Settings::Radar::on);
            ImGui::Checkbox("rotate", &Settings::Radar::rotate);
            break;
        case Tab::Extra:
            ImGui::Text("Extra");
            ImGui::Separator();

            ImGui::Checkbox("player name list", &Settings::NameList::on);
            ImGui::Checkbox("health locker", &Settings::HealthLocker::on);
            break;
        case Tab::Color:
            ImGui::Text("Color");
            ImGui::Separator();

            ImGui::ColorEdit4(
                    "teammate",
                    (float *)&Settings::Esp::teammateColor.Value
            );
            ImGui::ColorEdit4(
                    "enemy",
                    (float *)&Settings::Esp::enemyColor.Value
            );
            ImGui::ColorEdit4(
                    "full health",
                    (float *)&Settings::Esp::healthFullColor.Value
            );
            ImGui::ColorEdit4(
                    "half health",
                    (float *)&Settings::Esp::healthHalfColor.Value
            );
            ImGui::ColorEdit4(
                    "low health",
                    (float *)&Settings::Esp::healthLowColor.Value
            );
            break;
        default:
            break;
    }
    ImGui::EndChild();

    ImGui::End();
    ImGui::PopStyleVar();
}
