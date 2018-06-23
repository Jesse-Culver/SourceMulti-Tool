// Source Multi-Tool a program for helping Source Engine developers
// Building on top of the example code because why not
// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include "main_menu_bar.h"
#include "options_menu.h"
#include "tools_menu.h"

#include "SimpleIni.h"

#include <fstream>
#include <stdio.h>
#include <string>
#include <GL/gl3w.h>    // This is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <SDL.h>
#include <windows.h>

std::string ExePath();
bool verifyIni();

int main(int, char**) {
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
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
    SDL_Window *window = SDL_CreateWindow("Source Multi-Tool", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    gl3wInit();

    // Setup ImGui binding
    ImGui_ImplSdlGL3_Init(window);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //Setting Variables
    bool first_time = false;
    bool save_settings = false; //When this gets set to true we save our settings
    CSimpleIniA ini;    //This is our settings file variable
    char main_steam_directory[260] = "Please Insert";
    //Show Window Variables
    bool show_main_menu = true;
	bool show_options_menu = true;
    bool show_tools_menu = true;
    bool show_steamError_menu = false;
    
    //Checking if ini exists
    if(verifyIni()){
        char temp[260];
        strncpy(temp,ExePath().c_str(),128);
        strcat(temp, "/settings.ini");
        ini.LoadFile(temp);
        strcpy_s(main_steam_directory,260,ini.GetValue("main","steam",""));
    }
    else{
        first_time = true;
        show_tools_menu = false;
    }

    //SDL Variables that change because of client actions
    int sdl_width;
    int sdl_height;

    // Main loop
    bool done = false;
    while (!done) {
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSdlGL3_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }
        ImGui_ImplSdlGL3_NewFrame(window);

        //SDL_GetWindowSize(window, &sdl_width, &sdl_height);
        sdl_width = (int)ImGui::GetIO().DisplaySize.x;
        sdl_height = (int)ImGui::GetIO().DisplaySize.y;

        //Window code goes here
        showMainMenuBar(show_options_menu);
        ImGui::Begin("#mainmenu",&show_main_menu,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowSize(ImVec2(sdl_width, sdl_height));
        ImGui::SetWindowPos(ImVec2(0, 20));
        toolsMenu(show_tools_menu, *main_steam_directory);
        optionsMenu(show_options_menu, main_steam_directory, show_steamError_menu, save_settings);
        //Due to the way ImGui's ID stack works all popups must be called from the root while function
        //This is why we pass in bools to the other functions then if set to true get turned on below
        if(show_steamError_menu){
            ImGui::OpenPopup("ERROR01");
        }
        if(ImGui::BeginPopupModal("ERROR01",NULL,ImGuiWindowFlags_AlwaysAutoResize)){
            ImGui::Text("Steam Directory Provided Does not Contain steam.exe, therefore it is not the main steam directory!");
            if(ImGui::Button("OK")){
                show_steamError_menu = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if(first_time){
            ImGui::OpenPopup("FirstTimeSetup");
        }
        if(ImGui::BeginPopupModal("FirstTimeSetup",NULL,ImGuiWindowFlags_AlwaysAutoResize)){
            ImGui::Text("Welcome to Source Multi-Tool!");
            ImGui::Text("Before you can start you need to select your main steam directory in options");
            ImGui::Text("and save.");
            if(ImGui::Button("OK")){
                first_time = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if(save_settings){
            ini.SetValue("main", "steam", main_steam_directory);
            char temp[260];
            strncpy(temp,ExePath().c_str(),128);
            strcat(temp, "/settings.ini");
            ini.SaveFile(temp);
            show_tools_menu = true;
            save_settings = false;
        }

        ImGui::End();
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

//Credit to Eun on Stack Overflow https://stackoverflow.com/a/875264
std::string ExePath(){
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
    return std::string( buffer ).substr( 0, pos);
}

bool verifyIni(){
  char temp[256];
  strncpy(temp,ExePath().c_str(),128);
  strcat(temp, "/settings.ini");
  std::ifstream infile(temp);
  if(!infile){
    return false;
  }
  return true;
}