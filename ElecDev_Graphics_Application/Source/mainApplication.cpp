/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

// General.
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>

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
#include <../Graphics/graphicsHandler.h>

/*=======================================================================================================================================*/
/* Compiler settings.                                                                                                                    */
/*=======================================================================================================================================*/

// This removes the console that keeps opening with the app if it is not in debug mode.
#ifdef	_DEBUG
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
#else
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

/*=======================================================================================================================================*/
/* Variables/Globals/Defines.                                                                                                            */
/*=======================================================================================================================================*/

// Defined here, assigned in the main function where it has access to the window.
// Used as a global variable so that the mouse event callbacks from GLFW can have
// access to it.
GraphicsHandler* graphicsHandler;

/*=======================================================================================================================================*/
/* Functions.                                                                                                                            */
/*=======================================================================================================================================*/

// GLFW error handler.
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "[GLFW ERROR] %d: %s\n", error, description);
}

/*=======================================================================================================================================*/
/* Mouse events callbacks.                                                                                                               */
/*=======================================================================================================================================*/

// Handle mouse press events from GLFW.
void mousePressEvent(GLFWwindow* window, int button, int action, int mods)
{
    graphicsHandler->mousePressEvent(window, button, action, mods);
}

// Handle mouse press events from GLFW.
void mouseMoveEvent(GLFWwindow* window, double xpos, double ypos)
{
    graphicsHandler->mouseMoveEvent(window, xpos, ypos);
}

// Handle mouse press scroll. from GLFW.
void mouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{
    graphicsHandler->mouseScrollEvent(window, xoffset, yoffset);
}

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
        const char* glsl_version = "#version 450";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only.
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only.
    #endif

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    /* GLFW setup.                                                                                                                       */
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    // Enable 16x MSAA.
    glfwWindowHint(GLFW_SAMPLES, 16);
    // Create GLFW window.
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
        fprintf(stderr, "[OPENGL][ERROR] Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Print OpenGL version.
    std::cout << "[OPENGL][INFO] Loaded OpenGL version: " << glGetString(GL_VERSION) << ".\n\n";

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    // ImGUI & OpenGL setup. 
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;
    // Enable keyboard controls.
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup ImGui style.
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends.
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    glfwMakeContextCurrent(window);

    // Viewport.
    int screen_width, screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);
    glViewport(0, 0, screen_width, screen_height);
    // Variables used in loop.
    int display_w, display_h;

    // Setup mouse callbacks.
    glfwSetMouseButtonCallback(window, mousePressEvent);// Mouse press event.
    glfwSetCursorPosCallback(window, mouseMoveEvent);   //  Mouse move event.
    glfwSetScrollCallback(window, mouseScrollEvent);    // Mouse scroll event.

    // Enable MSAA.
    glEnable(GL_MULTISAMPLE);

    // Create the state machine variables.
    stateMachine* states = new stateMachine();
    states->toolsExpanded = false;
    states->toolsMode = 0;

    // Create graphics handler object.
    // For now a global variable is used to be able to have mouse callbacks with a method.
    // The callbacks cannot be used with a method, so it has to call a normal function.
    graphicsHandler = new GraphicsHandler(window, states);

    // Create GUI handler object.
    GUIHandler guiHandler(states, graphicsHandler);

    /*===================================================================================================================================*/
    /* Loop                                                                                                                              */
    /*===================================================================================================================================*/

    // [MISSING COMMENT]
    std::string interfacePython = "";

    // Graphics Pipeline
    while (!glfwWindowShouldClose(window))
    {
        // Check for events.
        //glfwWaitEvents();
        glfwPollEvents();
        // Init colors.
        glClear(GL_COLOR_BUFFER_BIT);

        // Handle graphics (OpenGL engines: Drawing and Designing).
        graphicsHandler->renderGraphics();

        // Feed inputs to ImGUI, start new frame.
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, 0.0f);

        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_NoDockingInCentralNode | ImGuiDockNodeFlags_PassthruCentralNode;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, work_size, dockspace_flags);
        //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_NoDockingInCentralNode| ImGuiDockNodeFlags_PassthruCentralNode);

        //Render ImGUI components.
        guiHandler.renderGraphics();

        /*std::cin >> interfacePython;
        ImGui::Begin("Interface Window Example");
        ImGui::Text("(%s)", interfacePython);
        ImGui::SetWindowPos(ImVec2(ImGui::GetMainViewport()->WorkSize.x - 160, 0));
        ImGui::SetNextWindowSize(ImVec2(30, 10));
        ImGui::End();*/

        //Render ImGUI into screen.
        ImGui::Render();
        // Assign values to viewport.
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        
        glfwSwapBuffers(window);
        //glFinish();

    }

    /*===================================================================================================================================*/

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