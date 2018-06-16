#include "imgui.h"
#include "options_menu.h"

#include <fstream>

void optionsMenu(bool& show_options_menu, char *main_steam_directory, bool& show_steamError_menu) {
  if(show_options_menu){
    if (ImGui::CollapsingHeader("Options")) {
      ImGui::Text("Main Steam Directory");
      ImGui::SameLine();
      ImGui::PushItemWidth(350);
      ImGui::InputText("##steamdirectoryinputtext", main_steam_directory, 128);
      ImGui::PopItemWidth();
      ImGui::SameLine();
      static ImGuiFs::Dialog fsInstance;
      const bool browseButtonPressed = ImGui::Button("Select Folder");
      static bool folderChosen = false; //This is so it only changes main_steam_directory once per select folder call
      if(browseButtonPressed) folderChosen = true;
      const char* chosenFolder = fsInstance.chooseFolderDialog(browseButtonPressed,fsInstance.getLastDirectory());
      if (strlen(fsInstance.getChosenPath())>0 && folderChosen) {
        strncpy(main_steam_directory,fsInstance.getChosenPath(),128);
        folderChosen = false;
      }
      if (ImGui::Button("Save Settings")){
        if(verifySteamDirectory(main_steam_directory)){
          //save settings
        }
        else{
         show_steamError_menu = true;
        }
      }
    }
  }
}

bool verifySteamDirectory(char *main_steam_directory){
  char temp[256];
  strncpy(temp,main_steam_directory,128);
  strcat(temp, "/steam.exe");
  std::ifstream infile(temp);
  if(!infile){
    return false;
  }
  return true;
}