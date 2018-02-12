//Example snippets

        // 1. Show a simple window.
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
                counter++;

            show_dock_window ^= ImGui::Button("ImGui Dock");
            
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }


        // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

		// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
        if (show_demo_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
            ImGui::ShowDemoWindow(&show_demo_window);
        }

		if (show_dock_window)   {
            if (ImGui::Begin("imguidock window (= lumix engine's dock system)",&show_dock_window,ImVec2(500, 500),0.95f,ImGuiWindowFlags_NoScrollbar)) {
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