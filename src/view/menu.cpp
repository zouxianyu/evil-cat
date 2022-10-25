#include <imgui.h>
#include "controller/controller.h"
#include "settings.h"
#include "menu.h"

void Menu::showInfo() {
    ImGui::Text("Welcome to use the FPS hacking framework: Evil Cat!");
    ImGui::Text("Press right shift to hide/open the menu.");
}

void Menu::show() {
    constexpr auto windowFlags = ImGuiWindowFlags_NoCollapse;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.f);
    ImGui::Begin("Evil Cat", &Settings::showMenu, windowFlags);

    if (ImGui::Button("Exit")) {
        ImGui::OpenPopup("Exit");
    }

    if (ImGui::CollapsingHeader("Info", ImGuiTreeNodeFlags_DefaultOpen)) {
        showInfo();
    }

    for (auto &[name, callback] : Controller::getInstance().getMenuCallbacks()) {
        if (ImGui::CollapsingHeader(name.c_str())) {
            callback();
        }
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

    ImGui::End();
    ImGui::PopStyleVar();
}
