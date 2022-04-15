//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/Application.h"
#include "Application/Events/EventLog.h"
#include "External/Misc/ConsoleColor.h"
#include "External/GLAD/Includes/glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "Resources/ResourceHandler.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "Utilities/Logger/Logger.h"
#include "glm/glm.hpp"
#include "Utilities/Profiler/Profiler.h"
#include "GLFW/glfw3.h"

//==============================================================================================================================================//
//  States.																																	    //
//==============================================================================================================================================//

// Mouse double press information.
#define MOUSE_DOUBLE_PRESS_TIMEOUT 0.4
static std::unordered_map<LumenEventID, double> s_buttonReleaseTimes;
static std::unordered_map<LumenEventID, bool> s_buttonReleaseIgnore;

// Mouse dragging information.
static bool s_draggingLeftbutton = false;
static glm::vec2 s_latestLeftButtonPressPosition;
static glm::vec2 s_mouseDragInitialPosition;
static glm::vec2 s_lastMouseMovePosition;

//==============================================================================================================================================//
//  Helpers.																																	//
//==============================================================================================================================================//

inline bool isEventOfType(LumenEventID eventID, LumenEventID compareID) 
{
    return (eventID & compareID) == compareID;
}

inline void resetDoublePressData() 
{
    for (auto& [button, time] : s_buttonReleaseTimes) time = 0;
    for (auto& [button, ignore] : s_buttonReleaseIgnore) ignore = false;
}

inline LumenEventID getMouseButtonState(GLFWwindow* window) 
{
    LumenEventID eventState = 0;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))     { eventState |= EventType_MouseButtonLeft;  }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))    { eventState |= EventType_MouseButtonRight; }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE))   { eventState |= EventType_MouseButtonMiddle;}
    return eventState;
}

inline LumenEventID getKeyState(GLFWwindow* window)
{
    LumenEventID eventState = 0;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))  { eventState |= EventType_LeftCtrl; }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { eventState |= EventType_RightCtrl; }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))    { eventState |= EventType_LeftShift; }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))   { eventState |= EventType_RightShift; }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))      { eventState |= EventType_LeftAlt; }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT))     { eventState |= EventType_RightAlt; }
    return eventState;
}

//==============================================================================================================================================//
//  Callbacks.																																	//
//==============================================================================================================================================//

void Application::glfwInitCallbacks()
{
    // Setup data.
    s_buttonReleaseTimes.insert({ EventType_MouseButtonLeft,   0 });
    s_buttonReleaseTimes.insert({ EventType_MouseButtonRight,  0 });
    s_buttonReleaseTimes.insert({ EventType_MouseButtonMiddle, 0 });
    s_buttonReleaseIgnore.insert({ EventType_MouseButtonLeft,   false });
    s_buttonReleaseIgnore.insert({ EventType_MouseButtonRight,  false });
    s_buttonReleaseIgnore.insert({ EventType_MouseButtonMiddle, false });

    // ------------------------- //
    //  M O U S E   B U T T O N  //
    // ------------------------- //

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
        {
            // Event state.
            LumenEventID eventState = getKeyState(window);

            // Get the button related to the event.
            LumenEventID currentButton = 0;
            if      (button == GLFW_MOUSE_BUTTON_LEFT)      { currentButton = EventType_MouseButtonLeft;   }
            else if (button == GLFW_MOUSE_BUTTON_RIGHT)     { currentButton = EventType_MouseButtonRight;  }
            else if (button == GLFW_MOUSE_BUTTON_MIDDLE)    { currentButton = EventType_MouseButtonMiddle; }
            eventState |= currentButton;    

            // Get the cursor position.
            double cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);

            // Create button event.
            LumenEventID buttonEventID = eventState;
            if      (action == GLFW_PRESS)      { buttonEventID |= EventType_MousePress; }
            else if (action == GLFW_RELEASE)    { buttonEventID |= EventType_MouseRelease; }

            // Double press.
            double currentTime = glfwGetTime();
            if ( (currentTime - s_buttonReleaseTimes[currentButton] < MOUSE_DOUBLE_PRESS_TIMEOUT) && isEventOfType(buttonEventID, EventType_MousePress))
            {
                // Remove mouse press ID so that double presses are seperate.
                buttonEventID &= ~(EventType_MousePress);
                buttonEventID |= EventType_MouseDoublePress;
                EventLog::log<MouseButtonEvent>(glm::vec2(cursorX, cursorY), buttonEventID);
                // Ignore next release for double press.
                s_buttonReleaseIgnore[currentButton] = true;
            }
            // Not a double press.
            else 
            {
                EventLog::log<MouseButtonEvent>(glm::vec2(cursorX, cursorY), buttonEventID);
            }

            // Pass event to ImGUI.
            ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

            // Store the latest pressed location for the mouse drag.
            if (isEventOfType(buttonEventID, EventType_MouseButtonLeft | EventType_MousePress) )
            {
                s_latestLeftButtonPressPosition = { cursorX, cursorY };
            }

            // Store the last time the button was released for tracking double click.
            if (isEventOfType(buttonEventID, EventType_MouseRelease))
            {    
                // Ignore release after a double press.
                if (s_buttonReleaseIgnore[currentButton])
                {
                    s_buttonReleaseTimes[currentButton] = 0;
                    s_buttonReleaseIgnore[currentButton] = false;
                }
                // Store release time.
                else
                {
                    // Reset times to ensure different mouse buttons stop double press.
                    for (auto& [button, time] : s_buttonReleaseTimes) time = 0;
                    // Set current time.
                    s_buttonReleaseTimes[currentButton] = currentTime;
                }
            }

            // Mouse drag event stops.
            if (isEventOfType(buttonEventID, EventType_MouseButtonLeft | EventType_MouseRelease) && s_draggingLeftbutton)
            {
                EventLog::logNotify(EventType_MouseDragStop | eventState);
                s_draggingLeftbutton = false;
            }
        });

    // ----------------------------------- //
    //  M O U S E   M O V E   &   D R A G  //
    // ----------------------------------- //

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
        {
            // Event states ID.
            LumenEventID eventState = getKeyState(window) | getMouseButtonState(window);

            // Get the cursor position.
            double cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            glm::vec2 mousePos(cursorX, cursorY);

            // Was not dragging, but left button is now pressed.
            if ( !s_draggingLeftbutton 
                && isEventOfType(eventState, EventType_MouseButtonLeft) )
            {
                EventLog::logNotify(EventType_MouseDragStart | eventState);
                s_draggingLeftbutton = true;
                s_mouseDragInitialPosition = s_latestLeftButtonPressPosition;
            }

            // If currently dragging, log an event.
            if (s_draggingLeftbutton && !isEventOfType(eventState, EventType_MouseButtonMiddle))
            {
                EventLog::logMouseDrag(s_mouseDragInitialPosition, mousePos, mousePos - s_lastMouseMovePosition, EventType_MouseDrag | eventState);
            }

            // Log move event.
            LumenEventID moveEventID = EventType_MouseMove | eventState;
            if (s_draggingLeftbutton)   moveEventID |= EventType_MouseDrag;
            EventLog::logMouseMove(mousePos, moveEventID);

            // Reset double press on a move.
            resetDoublePressData();

            // Store position.
            s_lastMouseMovePosition = mousePos;

            // Do not pass to imgui, Lumen handles this.
        });

    // ------------------------- //
    //  M O U S E   S C R O L L  //
    // ------------------------- //

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            // Event ID.
            LumenEventID eventID = getKeyState(window) | getMouseButtonState(window) | EventType_MouseScroll;

            // Get the cursor position.
            double cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);

            // Log event.
            EventLog::logMouseScroll(glm::vec2(cursorX, cursorY), (float)yoffset, (float)xoffset, eventID);

            // Reset double press on a scroll.
            resetDoublePressData();

            // Do not pass to imgui, Lumen handles this.
        });

    // ------- //
    //  K E Y  //
    // ------- //

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            // Event ID.
            LumenEventID eventID = getKeyState(window) | getMouseButtonState(window);
            if      (action == GLFW_PRESS)   { eventID |= EventType_KeyPress;   }
            else if (action == GLFW_RELEASE) { eventID |= EventType_KeyRelease; }
            else if (action == GLFW_REPEAT)  { eventID |= EventType_KeyRepeat;  }

            // Get the cursor position.
            double cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);

            // Log event.
            EventLog::log<KeyEvent>(key, eventID, glm::vec2(cursorX, cursorY));

            // Pass event to ImGUI.
            ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
        });
    
    // --------- //
    //  D R O P  //
    // --------- //

    glfwSetDropCallback(m_window, [](GLFWwindow* window, int count, const char** paths)
        {
            // Load the files.
            std::vector<std::filesystem::path> filePaths;
            filePaths.reserve(count);
            for (int i = 0; i < count; i++) 
                filePaths.emplace_back(std::string(paths[i])); 

            // Log the event.
            EventLog::log<FileDropEvent>(filePaths, EventType_Application);
        });

    // --------------------- //
    //  F B O   R E S I Z E  //
    // --------------------- //

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            EventLog::log<WindowEvent>(glm::vec2(width, height), EventType_Application | EventType_WindowResize);
        });

    // ----------- //
    //  C L O S E  //
    // ----------- //

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
        {
            Lumen::getApp().stopRunning();
        });
}

//==============================================================================================================================================//
//  Init.																																	    //
//==============================================================================================================================================//

GLFWwindow* Application::glfwInitWindow() 
{
    // Error setup.
    glfwSetErrorCallback([](int error, const char* description)
        {
            LUMEN_LOG_ERROR(error + description, "GLFW");
        });
    if (!glfwInit()) 
    { 
        LUMEN_LOG_ERROR("Could not initialise GLFW.", "GLFW");
    }

    // ----------------------------- //
    //  O P E N G L   V E R S I O N  //
    // ----------------------------- //

#if defined(__APPLE__)
    // GL 3.2 + GLSL 150.
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac.
#else
    // GL 3.0 + GLSL 130.
    const char* glsl_version = "#version 460";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only.
#endif

    // --------------------- //
    //  G L F W   S E T U P  //
    // --------------------- //

    // Enable MSAA.
    glfwWindowHint(GLFW_SAMPLES, 8);
    // Create GLFW window.
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Lumen", NULL, NULL);
    glfwSetTime(0);
    // Error.
    if (window == NULL) 
    { 
        LUMEN_LOG_ERROR("Could not create GLFW window.", "GLFW");
    }
    glfwMakeContextCurrent(window);
    // VSync (0 = disabled).
    glfwSwapInterval(0);

    // Load GLFW app icon.
    BITMAP bitmap = loadImageFromResource(ICON_PNG);
    GLFWimage icon;
    // Load bitmap data to GLFW icon.
    icon.pixels = (unsigned char*)bitmap.bmBits;
    icon.height = bitmap.bmHeight;
    icon.width = bitmap.bmWidth;
    // Set icon.
    glfwSetWindowIcon(window, 1, &icon);

    // --------------------------- //
    //  O P E N G L   L O A D E R  //
    // --------------------------- //

    // Load OpenGL functions using GLAD.
    if(!gladLoadGL())
    {
        LUMEN_LOG_ERROR("Failed to initialize OpenGL loader", "GLAD");
    }

    // Log OpenGL version.
    std::string msg = "Loaded OpenGL version '";
    std::string version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    Lumen::getApp().pushNotification(NotificationType::Info, 4000, msg + version + "'.", "Renderer");
    LUMEN_LOG_INFO(msg + version + "'.", "Renderer");

    // ----------------------------------------- //
    //  I M G U I   &   O P E N G L   S E T U P  // 
    // ----------------------------------------- //

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;                                      // Disable imgui ini file.
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable keyboard controls.
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    // Multi viewports need some work with GLFW to be implemented.
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // When viewports are enabled we tweak WindowRounding/WindowBg 
    // so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 5.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends.
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return window;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
