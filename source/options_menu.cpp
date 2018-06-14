#include "imgui.h"

void optionsMenu(bool& show_options_menu, char& main_steam_directory) {
  if(show_options_menu){
    if (ImGui::CollapsingHeader("Options")) {
      ImGui::Text("Main Steam Directory");
      ImGui::SameLine();
      ImGui::PushItemWidth(350);
      ImGui::InputText("##steamdirectoryinputtext", &main_steam_directory, 128);
      ImGui::PopItemWidth();
      ImGui::SameLine();
      if (ImGui::Button("Browse")) {
      }
      if (ImGui::Button("Save Settings")){
      }
    }
  }
}