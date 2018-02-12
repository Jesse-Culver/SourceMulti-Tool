// Source Multi-Tool a program for helping Source Engine developers
// Building on top of the example code because why not
// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>    // This is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <SDL.h>

static void ShowMainMenuBar();
static void ShowDockingWindows(bool show_dock_window);

int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_Window *window = SDL_CreateWindow("Source Multi-Tool", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    gl3wInit();

    // Setup ImGui binding
    ImGui_ImplSdlGL3_Init(window);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool show_dock_window = true;

    // Main loop
    bool done = false;
    while (!done)
    {
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSdlGL3_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }
        ImGui_ImplSdlGL3_NewFrame(window);

        //Window code goes here
        ShowMainMenuBar();
        ShowDockingWindows(show_dock_window);

        // Rendering
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplSdlGL3_Shutdown();
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


static void ShowMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

static void ShowDockingWindows(bool show_dock_window) {
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
}