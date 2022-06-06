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
#include <unordered_map>
#include "GLFW/glfw3.h"

//==============================================================================================================================================//
//  States.																																	    //
//==============================================================================================================================================//

// Mouse double press information.
#define MOUSE_DOUBLE_PRESS_TIMEOUT 0.3
std::unordered_map<LumenEventID, double> s_buttonReleaseTimes;
std::unordered_map<LumenEventID, bool> s_buttonReleaseIgnore;

// Mouse dragging information.
glm::vec2 s_latestMouseButtonPressPosition;
glm::vec2 s_lastMouseMovePosition;
LumenEventID s_currentlyDraggingButton = NULL;
std::unordered_map<LumenEventID, bool> s_buttonDragActive;
std::unordered_map<LumenEventID, glm::vec2> s_buttonDragInitialPos;
std::vector<LumenEventID> s_heldButtons;

//==============================================================================================================================================//
//  Helpers.																																	//
//==============================================================================================================================================//

inline bool isIdOfType(LumenEventID eventID, LumenEventID compareID) 
{
    return (eventID & compareID) == compareID;
}

inline void resetDoublePressReleaseIgnore() 
{
    for (auto& pair : s_buttonReleaseIgnore) pair.second = false;
}

inline void resetDoublePressReleaseTimes() 
{
    for (auto& pair : s_buttonReleaseTimes)  pair.second = 0;
}

inline void resetDoublePressData() 
{
    resetDoublePressReleaseTimes();
    resetDoublePressReleaseIgnore();
}

inline void removeHeldButton(LumenEventID buttonToRemove) 
{
    int index = 0;
    for (auto& button : s_heldButtons)
    {
        if (button == buttonToRemove)
        {
            s_heldButtons.erase(s_heldButtons.begin() + index);
            break;
        }
        index += 1;
    }
}

inline void addHeldButton(LumenEventID button) 
{
    s_heldButtons.push_back(button);
}

inline LumenEventID getLatestHeldButton() 
{
    return s_heldButtons.back();
}

inline LumenEventID getMouseButtonState(GLFWwindow* window) 
{
    LumenEventID eventState = 0;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))     { eventState |= EventType_MouseButtonLeft;   }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))    { eventState |= EventType_MouseButtonRight;  }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE))   { eventState |= EventType_MouseButtonMiddle; }
    return eventState;
}

inline LumenEventID getKeyState(GLFWwindow* window)
{
    LumenEventID eventState = 0;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))  { eventState |= EventType_LeftCtrl;   }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { eventState |= EventType_RightCtrl;  }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))    { eventState |= EventType_LeftShift;  }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))   { eventState |= EventType_RightShift; }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))      { eventState |= EventType_LeftAlt;    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT))     { eventState |= EventType_RightAlt;   }
    return eventState;
}

// GLFW passes mouse coodinates with (0,0) as the top left.  Lumen has (0,0) in the bottom left,
// so the coordinates are converted here.
inline glm::vec2 getLumenCursorPos(GLFWwindow* window) 
{
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    return { cursorX, display_h - cursorY };
}

//==============================================================================================================================================//
//  Callbacks.																																	//
//==============================================================================================================================================//

void Application::glfwInitCallbacks()
{
    // Setup double press data.
    s_buttonReleaseTimes[EventType_MouseButtonLeft]     = 0;
    s_buttonReleaseTimes[EventType_MouseButtonRight]    = 0;
    s_buttonReleaseTimes[EventType_MouseButtonMiddle]   = 0;
    s_buttonReleaseIgnore[EventType_MouseButtonLeft]    = false;
    s_buttonReleaseIgnore[EventType_MouseButtonRight]   = false;
    s_buttonReleaseIgnore[EventType_MouseButtonMiddle]  = false;

    // Setup dragging data.
    s_buttonDragActive[EventType_MouseButtonLeft]       = false;
    s_buttonDragActive[EventType_MouseButtonMiddle]     = false;
    s_buttonDragActive[EventType_MouseButtonRight]      = false;
    s_buttonDragInitialPos[EventType_MouseButtonLeft]   = { 0.f, 0.f };
    s_buttonDragInitialPos[EventType_MouseButtonMiddle] = { 0.f, 0.f };
    s_buttonDragInitialPos[EventType_MouseButtonRight]  = { 0.f, 0.f };
    s_heldButtons.reserve(3);

    // ------------------------- //
    //  M O U S E   B U T T O N  //
    // ------------------------- //

    glfwSetMouseButtonCallback(getGLFWWindow(), [](GLFWwindow* window, int button, int action, int mods)
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
            glm::vec2 cursorPos = getLumenCursorPos(window);

            // Create button event.
            LumenEventID buttonEventID = eventState;
            if      (action == GLFW_PRESS)      { buttonEventID |= EventType_MousePress;   }
            else if (action == GLFW_RELEASE)    { buttonEventID |= EventType_MouseRelease; }

            // Check for double press.
            double currentTime = glfwGetTime();
            if ((currentTime - s_buttonReleaseTimes[currentButton] < MOUSE_DOUBLE_PRESS_TIMEOUT) && isIdOfType(buttonEventID, EventType_MousePress))
            {
                // Remove mouse press ID so that double presses are seperate.
                buttonEventID = (buttonEventID & ~EventType_MousePress) | EventType_MouseDoublePress;
                EventLog::log<MouseButtonEvent>(cursorPos, buttonEventID);
                // Ignore next release for double press.
                s_buttonReleaseIgnore[currentButton] = true;
            }
            // Not a double press.
            else 
            {
                EventLog::log<MouseButtonEvent>(cursorPos, buttonEventID);
            }

            // Store the latest pressed location for the mouse drag.
            if (isIdOfType(buttonEventID, EventType_MousePress))
            {
                s_latestMouseButtonPressPosition = cursorPos;
                addHeldButton(currentButton); 
            }

            // Dragging & double press.
            if (isIdOfType(buttonEventID, EventType_MouseRelease))
            {    
                // Ignore release after a double press to restart counter.
                if (s_buttonReleaseIgnore[currentButton])
                {
                    s_buttonReleaseTimes[currentButton] = 0;
                    s_buttonReleaseIgnore[currentButton] = false;
                }
                // Store release time if not double press.
                else
                {
                    // Reset times to ensure different mouse buttons stop double press & set current time.
                    resetDoublePressReleaseTimes();
                    s_buttonReleaseTimes[currentButton] = currentTime;
                }

                // Dragging event stops on button release.
                if (s_buttonDragActive[currentButton])
                {
                    EventLog::logNotify(EventType_MouseDragStop | eventState);
                    s_buttonDragActive[currentButton] = false;
                    s_currentlyDraggingButton = NULL;
                }

                removeHeldButton(currentButton);
            }

            // Pass event to ImGUI.
            ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        });

    // ----------------------------------- //
    //  M O U S E   M O V E   &   D R A G  //
    // ----------------------------------- //

    glfwSetCursorPosCallback(getGLFWWindow(), [](GLFWwindow* window, double xpos, double ypos)
        {
            // Event states ID.
            LumenEventID keyState = getKeyState(window);
            LumenEventID buttonState = getMouseButtonState(window);

            // Get the cursor position.
            glm::vec2 mousePos = getLumenCursorPos(window);

            // Dragging logic.
            if(s_heldButtons.size())
            {
                // Get the button that should be dragging.
                LumenEventID currentButton = getLatestHeldButton();

                // On button draging change / start.
                if (s_currentlyDraggingButton != currentButton)
                {
                    // Log the start and set data.
                    if (!s_buttonDragActive[currentButton])
                    {
                        EventLog::logNotify(EventType_MouseDragStart | keyState | currentButton);
                        s_buttonDragActive[currentButton] = true;
                        s_buttonDragInitialPos[currentButton] = s_latestMouseButtonPressPosition;
                    }
                    
                    // Update dragging button.
                    s_currentlyDraggingButton = currentButton;
                }

                // Is currently dragging.
                if (currentButton == s_currentlyDraggingButton)
                {
                    EventLog::logMouseDrag(s_buttonDragInitialPos[currentButton], mousePos, mousePos - s_lastMouseMovePosition, EventType_MouseDrag | keyState | s_currentlyDraggingButton);
                }
            }
            
            // Log move event.
            LumenEventID moveEventID = EventType_MouseMove | keyState | buttonState;
            if (s_currentlyDraggingButton)    
                moveEventID |= EventType_MouseDrag;
            EventLog::logMouseMove(mousePos, moveEventID);

            // Double press should not occur after move.
            resetDoublePressData();

            // Store position.
            s_lastMouseMovePosition = mousePos;

            // Do not pass to imgui, Lumen handles this.
        });

    // ------------------------- //
    //  M O U S E   S C R O L L  //
    // ------------------------- //

    glfwSetScrollCallback(getGLFWWindow(), [](GLFWwindow* window, double xoffset, double yoffset)
        {
            // Event ID.
            LumenEventID eventID = getKeyState(window) | getMouseButtonState(window) | EventType_MouseScroll;

            // Get the cursor position.
            glm::vec2 cursorPos = getLumenCursorPos(window);

            // Log event.
            EventLog::logMouseScroll(cursorPos, (float)yoffset, (float)xoffset, eventID);

            // Double press should not log after a scroll.
            resetDoublePressData();

            // Do not pass to imgui, Lumen handles this.
        });

    // ------- //
    //  K E Y  //
    // ------- //

    glfwSetKeyCallback(getGLFWWindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            // Event ID.
            LumenEventID eventID = getKeyState(window) | getMouseButtonState(window);
            if      (action == GLFW_PRESS)   { eventID |= EventType_KeyPress;   }
            else if (action == GLFW_RELEASE) { eventID |= EventType_KeyRelease; }
            else if (action == GLFW_REPEAT)  { eventID |= EventType_KeyRepeat;  }

            // Get the cursor position.
            glm::vec2 cursorPos = getLumenCursorPos(window);

            // Log event.
            EventLog::log<KeyEvent>(key, eventID, cursorPos);

            // Pass event to ImGUI.
            ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
        });
    
    // --------- //
    //  D R O P  //
    // --------- //

    glfwSetDropCallback(getGLFWWindow(), [](GLFWwindow* window, int count, const char** paths)
        {
            // Load the files.
            std::vector<std::filesystem::path> filePaths;
            filePaths.reserve(count);
            for (int i = 0; i < count; i++) 
                filePaths.emplace_back(std::filesystem::path(paths[i])); 

            // Log the event.
            EventLog::log<FileDropEvent>(filePaths, EventType_Application);
        });

    // --------------------- //
    //  F B O   R E S I Z E  //
    // --------------------- //

    glfwSetFramebufferSizeCallback(getGLFWWindow(), [](GLFWwindow* window, int width, int height)
        {
            EventLog::log<WindowEvent>(glm::vec2(width, height), EventType_Application | EventType_WindowResize);
        });

    // ----------- //
    //  C L O S E  //
    // ----------- //

    glfwSetWindowCloseCallback(getGLFWWindow(), [](GLFWwindow* window)
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

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

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
