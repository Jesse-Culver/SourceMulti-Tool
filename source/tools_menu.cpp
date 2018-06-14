#include "imgui.h"

void toolsMenu(bool& show_tools_menu, char& main_steam_directory) {
  if(show_tools_menu){
    if (ImGui::CollapsingHeader("Tools")) {
      if (ImGui::Button("Hammer")) {
      }
      if (ImGui::Button("Model Viewer")) {
      }
    }
  }
}