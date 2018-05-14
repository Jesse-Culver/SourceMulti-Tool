//Menu System
#include "imgui.h"

void MenuSystem(bool show_menu_system, int width, int height) {
if (show_menu_system)   {
            if (ImGui::Begin("##main menu",&show_menu_system,ImVec2(width, height),0.95f,ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {
				ImGui::SetWindowSize(ImVec2(width,height), ImGuiCond_Always);
                ImGui::BeginDockspace();
                static char tmp[128];
                for (int i=0;i<10;i++)  {
                    sprintf(tmp,"Dock %d",i);
                    if (i==9) ImGui::SetNextDock(ImGuiDockSlot_Bottom);// optional
                    if(ImGui::BeginDock(tmp))  {
                        ImGui::Text("Content of dock window %d goes here",i);
                    }
                    ImGui::EndDock();
                }
    //=========== OPTIONAL STUFF ===================================================
                static bool draggingLookOpen = true;    // With this next dock has a close button (but its state is not serializable AFAIK)
                // We're also passing a 'default_size' as initial size of the window once undocked
                if (ImGui::BeginDock("Dragging Look",&draggingLookOpen,0,ImVec2(200,350)))    {
                    ImGui::Checkbox("Textured##imguidockDraggingLook",&gImGuiDockReuseTabWindowTextureIfAvailable);
                }
                ImGui::EndDock();
    //===========END OPTIONAL STUFF =================================================
    //========== OPTIONAL STUFF =====================================================
    #           if (!defined(NO_IMGUIHELPER) && !defined(NO_IMGUIHELPER_SERIALIZATION))
                if (ImGui::BeginDock("Load/Save"))  {
                    static const char* saveName = "myDock.layout";
                    const char* saveNamePersistent = "/persistent_folder/myDock.layout";
                    const char* pSaveName = saveName;
    #               ifndef NO_IMGUIHELPER_SERIALIZATION_SAVE
                    if (ImGui::Button("Save")) {
    #                   ifdef YES_IMGUIEMSCRIPTENPERSISTENTFOLDER
                        pSaveName = saveNamePersistent;
    #                   endif //YES_IMGUIEMSCRIPTENPERSISTENTFOLDER
                        if (ImGui::SaveDock(pSaveName))   {
    #                       ifdef YES_IMGUIEMSCRIPTENPERSISTENTFOLDER
                            ImGui::EmscriptenFileSystemHelper::Sync();
    #                       endif //YES_IMGUIEMSCRIPTENPERSISTENTFOLDER
                        }
                    }
                    ImGui::SameLine();
    #               endif //NO_IMGUIHELPER_SERIALIZATION_SAVE
    #               ifndef NO_IMGUIHELPER_SERIALIZATION_LOAD
                    if (ImGui::Button("Load")) {
    #                   ifdef YES_IMGUIEMSCRIPTENPERSISTENTFOLDER
                        if (ImGuiHelper::FileExists(saveNamePersistent)) pSaveName = saveNamePersistent;
    #                   endif //YES_IMGUIEMSCRIPTENPERSISTENTFOLDER
                        ImGui::LoadDock(pSaveName);
                    }
                    ImGui::SameLine();
    #               endif //NO_IMGUIHELPER_SERIALIZATION_LOAD
                }
                ImGui::EndDock();   //Load/Save
    #           endif //NO_IMGUIHELPER_SERIALIZATION
    //=========== END OPTIONAL STUFF ================================================
                ImGui::EndDockspace();
            }
            ImGui::End();
        }
} 