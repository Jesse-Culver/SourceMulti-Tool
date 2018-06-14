//Main Menu Bar
#include "imgui.h"

void showMainMenuBar(bool& show_options_menu) {
if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Options", NULL, show_options_menu)) {
                show_options_menu = !show_options_menu;
            }
            if (ImGui::MenuItem("About")) {}
            if (ImGui::MenuItem("Exit")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
