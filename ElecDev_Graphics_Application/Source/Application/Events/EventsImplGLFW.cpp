//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "glad/glad.h"
#include "EventsImplGLFW.h"
#include "Events.h"
#include "Misc/ConsoleColor.h"
#include "ImGUI/Implementations/imgui_impl_glfw.h"
#include "ImGUI/Implementations/imgui_impl_opengl3.h"
#include "Resources/ResourceHandler.h"
#include "CoreGL/ErrorHandlerGL.h"

//==============================================================================================================================================//
//  GLFW events.																														        //
//==============================================================================================================================================//

// GLFW error handler.
void glfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, (const char*)red, "\n\n[GLFW] [ERROR] : ", (const char*)white, "%d: %s\n", error, description);
}

void glfwMouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	// Create the event ID.
	uint64_t eventID = 0;

	// Event type.
	if		(action == GLFW_PRESS)	 { eventID |= EventType::MousePress; }
	else if (action == GLFW_RELEASE) { eventID |= EventType::MouseRelease; }

	// Button states.
	if		(button == GLFW_MOUSE_BUTTON_LEFT)   { eventID |= EventType::MouseButtonLeft; }
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)  { eventID |= EventType::MouseButtonRight; }
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) { eventID |= EventType::MouseButtonMiddle; }

    // Get the cursos position.
    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    glm::vec2 cursorPos = { cursorX, cursorY };

	// Log event.
	MouseButtonEvent event(cursorPos, eventID);
	eventLog.log(event);

	// ImGui mouse callback.
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

void glfwMouseMoveEvent(GLFWwindow* window, double xpos, double ypos)
{
	// Event ID.
	uint64_t eventID = EventType::MouseMove;

	// Set button states.
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))   { eventID |= EventType::MouseButtonLeft; }
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))  { eventID |= EventType::MouseButtonRight; }
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) { eventID |= EventType::MouseButtonMiddle; }

	// Get the cursos position.
    double cursorX, cursorY;
	glfwGetCursorPos(window, &cursorX, &cursorY);
    glm::vec2 cursorPos = {cursorX, cursorY};

	// Log event.
	MouseMoveEvent event(cursorPos, eventID);
	eventLog.log(event);
}

void glfwMouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{
	// Event ID.
	uint64_t eventID = EventType::MouseScroll;

	// Set button states.
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))   { eventID |= EventType::MouseButtonLeft; }
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))  { eventID |= EventType::MouseButtonRight; }
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) { eventID |= EventType::MouseButtonMiddle; }

    // Get the cursos position.
    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    glm::vec2 cursorPos = { cursorX, cursorY };

	// Log event.
	MouseScrollEvent event(cursorPos, yoffset, eventID);
	eventLog.log(event);

	// ImGui scroll callback.
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

void glfwKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Event ID.
	uint64_t eventID = 0;

	// Event type.
	if		(action == GLFW_PRESS)   { eventID |= EventType::KeyPress; }
	else if (action == GLFW_RELEASE) { eventID |= EventType::KeyRelease; }

	// Log event.
	KeyEvent event(key, eventID);
	eventLog.log(event);

	// ImGui key callback.
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

void glfwFileDropEvent(GLFWwindow* window, int count, const char** paths)
{
	/*graphicsHandler->m_fileDropEvent.eventTrigger = true;
	graphicsHandler->m_fileDropEvent.totalFiles = count;
	for (int i = 0; i < count; i++) { graphicsHandler->m_fileDropEvent.paths.push_back(paths[i]); }*/
}

//==============================================================================================================================================//
//  GLFW setup.																														            //
//==============================================================================================================================================//

GLFWwindow* initWindow()
{
    // Setup window.
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit())
    {
        // Log error here.
    }

    // ----------------------------- //
    //  O P E N G L   V E R S I O N  //
    // ----------------------------- //

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
    const char* glsl_version = "#version 460";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only.
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only.
#endif

    // --------------------- //
    //  G L F W   S E T U P  //
    // --------------------- //

    // Enable 8x MSAA.
    glfwWindowHint(GLFW_SAMPLES, 8);
    // Create GLFW window.
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Lumen", NULL, NULL);
    if (window == NULL)
    {
        // Log error here.
    }
    glfwMakeContextCurrent(window);
    // VSync.  If set to 0 it is disabled.
    glfwSwapInterval(0);

    // Load GLFW icon.
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

    // Check for OpenGL load error.
    if (err)
    {
        fprintf(stderr, (const char*)red, "\n\n[OPENGL] [ERROR] : ", (const char*)white, " Failed to initialize OpenGL loader!\n");
        // Log error here.;
    }

    // Print OpenGL version.
    std::cout << blue << "\n[OPENGL] [INFO] : " << white << " Loaded OpenGL version " << glGetString(GL_VERSION) << ".";

    // ----------------------------------------- //
    //  I M G U I   &   O P E N G L   S E T U P  // 
    // ----------------------------------------- //

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;                                    // Disable imgui ini file.
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable keyboard controls.
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // When viewports are enabled we tweak WindowRounding/WindowBg 
    // so platform windows can look identical to regular ones.
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
    GLCall(glViewport(0, 0, screen_width, screen_height));

    // Setup GLFW event callbacks.
    glfwSetMouseButtonCallback(window, glfwMouseButtonEvent);       // Mouse button event.
    glfwSetCursorPosCallback(window, glfwMouseMoveEvent);           // Mouse move event.
    glfwSetScrollCallback(window, glfwMouseScrollEvent);            // Mouse scroll event.
    glfwSetKeyCallback(window, glfwKeyEvent);                       // Key press event.
    glfwSetDropCallback(window, glfwFileDropEvent);                 // For when a file is dopped into the app.

    return window;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
