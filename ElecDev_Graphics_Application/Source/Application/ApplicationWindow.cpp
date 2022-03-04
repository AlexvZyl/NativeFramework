//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/Application.h"
#include "Lumen.h"
#include "External/Misc/ConsoleColor.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "Resources/ResourceHandler.h"
#include "External/GLAD/Includes/glad/glad.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

//==============================================================================================================================================//
//  Callbacks.																																	//
//==============================================================================================================================================//

void Application::glfwInitCallbacks()
{
    // ------------------------- //
    //  M O U S E   B U T T O N  //
    // ------------------------- //

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
        {
            // Create the event ID. 
            uint64_t eventID = 0;
            if      (action == GLFW_PRESS)               { eventID |= EventType_MousePress;        }
            else if (action == GLFW_RELEASE)             { eventID |= EventType_MouseRelease;      }
            if      (button == GLFW_MOUSE_BUTTON_LEFT)   { eventID |= EventType_MouseButtonLeft;   }
            else if (button == GLFW_MOUSE_BUTTON_RIGHT)  { eventID |= EventType_MouseButtonRight;  }
            else if (button == GLFW_MOUSE_BUTTON_MIDDLE) { eventID |= EventType_MouseButtonMiddle; }
            // Key states.
            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))  { eventID |= EventType_LeftCtrl;   }
            if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { eventID |= EventType_RightCtrl;  }
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))    { eventID |= EventType_LeftShift;  }
            if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))   { eventID |= EventType_RightShift; }
            if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))      { eventID |= EventType_LeftAlt;    }
            if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT))     { eventID |= EventType_RightAlt;   }

            // Get the cursor position.
            double cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);

            // Log event.
            MouseButtonEvent event(glm::vec2(cursorX, cursorY), eventID);
            Lumen::getApp().logEvent<MouseButtonEvent>(event);

            // Pass event to ImGUI.
            ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        });

    // --------------------- //
    //  M O U S E   M O V E  //
    // --------------------- //

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
        {
            // Event ID.
            uint64_t eventID = EventType_MouseMove;
            // Mouse button states.
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))   { eventID |= EventType_MouseButtonLeft;   }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))  { eventID |= EventType_MouseButtonRight;  }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) { eventID |= EventType_MouseButtonMiddle; }
            // Key states.
            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))  { eventID |= EventType_LeftCtrl;   }
            if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { eventID |= EventType_RightCtrl;  }
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))    { eventID |= EventType_LeftShift;  }
            if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))   { eventID |= EventType_RightShift; }
            if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))      { eventID |= EventType_LeftAlt;    }
            if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT))     { eventID |= EventType_RightAlt;   }

            // Get the cursor position.
            double cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);

            // Log event.
            MouseMoveEvent event(glm::vec2(cursorX, cursorY), eventID);
            Lumen::getApp().logEvent<MouseMoveEvent>(event);

            // Pass event to ImGUI.
            ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
        });

    // ------------------------- //
    //  M O U S E   S C R O L L  //
    // ------------------------- //

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            // Event ID.
            uint64_t eventID = EventType_MouseScroll;
            // Mouse button states.
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))   { eventID |= EventType_MouseButtonLeft;   }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))  { eventID |= EventType_MouseButtonRight;  }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) { eventID |= EventType_MouseButtonMiddle; }
            // Key states.
            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))  { eventID |= EventType_LeftCtrl;   }
            if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { eventID |= EventType_RightCtrl;  }
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))    { eventID |= EventType_LeftShift;  }
            if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))   { eventID |= EventType_RightShift; }
            if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))      { eventID |= EventType_LeftAlt;    }
            if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT))     { eventID |= EventType_RightAlt;   }

            // Get the cursor position.
            double cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);

            // Log event.
            MouseScrollEvent event(glm::vec2(cursorX, cursorY), yoffset, eventID);
            Lumen::getApp().logEvent<MouseScrollEvent>(event);

            // Pass event to ImGUI.
            ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
        });

    // ------- //
    //  K E Y  //
    // ------- //

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            // Event ID.
            uint64_t eventID = 0;
            if      (action == GLFW_PRESS)   { eventID |= EventType_KeyPress;   }
            else if (action == GLFW_RELEASE) { eventID |= EventType_KeyRelease; }
            else if (action == GLFW_REPEAT)  { eventID |= EventType_KeyRepeat;  }
            // Mouse button states.
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))   { eventID |= EventType_MouseButtonLeft;   }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))  { eventID |= EventType_MouseButtonRight;  }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) { eventID |= EventType_MouseButtonMiddle; }
            // Key states.
            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))  { eventID |= EventType_LeftCtrl;   }
            if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { eventID |= EventType_RightCtrl;  }
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))    { eventID |= EventType_LeftShift;  }
            if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))   { eventID |= EventType_RightShift; }
            if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))      { eventID |= EventType_LeftAlt;    }
            if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT))     { eventID |= EventType_RightAlt;   }

            // Get the cursor position.
            double cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);

            // Log event.
            KeyEvent event(key, eventID, glm::vec2(cursorX, cursorY));
            Lumen::getApp().logEvent<KeyEvent>(event);

            // Pass event to ImGUI.
            ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
        });
    
    // --------- //
    //  D R O P  //
    // --------- //

    glfwSetDropCallback(m_window, [](GLFWwindow* window, int count, const char** paths)
        {
            // Load the files.
            std::vector<std::string> filePaths;
            filePaths.reserve(count);
            for (int i = 0; i < count; i++) { filePaths.emplace_back(std::string(paths[i])); }

            // Log the event.
            FileDropEvent event(filePaths);
            Lumen::getApp().logEvent<FileDropEvent>(event);
        });

    // --------------------- //
    //  F B O   R E S I Z E  //
    // --------------------- //

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            // Create and log event.
            WindowEvent event(glm::vec2(width, height), EventType_Application | EventType_WindowResize);
            Lumen::getApp().logEvent<WindowEvent>(event);
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
    // Setup window.
    glfwSetErrorCallback([](int error, const char* description)
    {
        fprintf(stderr, (const char*)red, "\n\n[GLFW] [ERROR] : ", (const char*)white, "%d: %s\n", error, description);
    });
    // Error.
    if (!glfwInit()) { /* Log error here. */ }

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
    GLFWwindow* window = glfwCreateWindow(1280, 720, "", NULL, NULL);
    // Error.
    if (window == NULL) 
    { 
        /* Log error here. */ 
    }
    // Remove decorations.
    //glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
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
    //glfwSetWindowIcon(window, 1, &icon);

    // --------------------------- //
    //  O P E N G L   L O A D E R  //
    // --------------------------- //

    // Load OpenGL functions using GLAD.
    if(!gladLoadGL())
    {
        // Log error.
        fprintf(stderr, (const char*)red, "\n\n[OPENGL] [ERROR] : ", (const char*)white, " Failed to initialize OpenGL loader!\n");
    }

    // Log OpenGL version.
    std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Loaded OpenGL version " << glGetString(GL_VERSION) << ".";

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
