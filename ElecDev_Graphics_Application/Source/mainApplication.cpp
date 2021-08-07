/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

// General.
#include <stdio.h>
#include <vector>
#include <iostream>

// ImGUI (GUI software). 
#include "Core/imgui.h"
#include "Implementations/imgui_impl_glfw.h"
#include "Implementations/imgui_impl_opengl3.h"

// GLAD (OpenGL loader).
#include <glad/glad.h>

// Include GLFW (window) after OpenGL definition.
#include <GLFW/glfw3.h>

// GUI includes.
#include "GUI/guiHandler.h"

// Graphics handler include.
#include "Graphics/graphicsHandler.h"

/*=======================================================================================================================================*/
/* Functions.                                                                                                                            */
/*=======================================================================================================================================*/

// GLFW error handler.
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "[GLFW ERROR] %d: %s\n", error, description);
}

// This removes the console that keeps opening with the app if it is not in debug mode.
#ifdef	_DEBUG
    #pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
#else
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

/*=======================================================================================================================================*/
/* Main                                                                                                                                  */
/*=======================================================================================================================================*/

int main(int, char**)
{
    // Setup window.
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    // OpenGL Version select.
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    #if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100.
        const char* glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    #elif defined(__APPLE__)
        // GL 3.2 + GLSL 150.
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only.
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac.
    #else
        // GL 3.0 + GLSL 130.
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only.
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only.
    #endif

    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ElecDev Graphics", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync.

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    // Initialize OpenGL loader.
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    #if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
        bool err = gl3wInit() != 0;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
        bool err = glewInit() != GLEW_OK;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
        bool err = gladLoadGL() == 0;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
        bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
        bool err = false;
        glbinding::Binding::initialize();
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
        bool err = false;
        glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
    #else
        bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
    #endif

    // OpenGL loader error handler.
    if (err)
    {
        fprintf(stderr, "[OPENGL ERROR] Failed to initialize OpenGL loader!\n");
        return 1;
    }

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    // ImGUI setup. 
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    // Create GUI handler object.
    GUIHandler guiHandler;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls.

    // Setup ImGui style.
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();  // Set mode to light.

    // Setup Platform/Renderer backends.
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    // OpenGL setup.
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    // Viewport.
    int screen_width, screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);
    glViewport(0, 0, screen_width, screen_height);
    // Variables used in loop.
    int display_w, display_h;

    // Create graphics handler object.
    GraphicsHandler graphicsHandler(window);
    
    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    // Other inits.
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    // Set background color.
    float backGroundColor[] = { (float) 245 / 255, (float) 245 / 255, (float) 245 / 255 };

    /*===================================================================================================================================*/
    /* Loop                                                                                                                              */
    /*===================================================================================================================================*/

    // Graphics Pipeline
    while (!glfwWindowShouldClose(window))
    {   
        // Check for events.
        glfwPollEvents();
        // Init colors.
        glClearColor(backGroundColor[0], backGroundColor[1], backGroundColor[2], 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Handle graphics (OpenGL engines: Drawing and Designing).
        graphicsHandler.renderGraphics();

        // Feed inputs to ImGUI, start new frame.
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render Toolbar.
        guiHandler.renderGraphics();

        // Render ImGUI into screen.
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Viewport.
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glfwSwapBuffers(window);
        
    }

    /*s===================================================================================================================================*/

    // Cleanup.
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Close application.
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;

}

/*=======================================================================================================================================*/
/* EOF                                                                                                                                   */
/*=======================================================================================================================================*/