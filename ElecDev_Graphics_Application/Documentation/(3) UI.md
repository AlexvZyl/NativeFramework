# GUI

Creating a `GUI` is very similar to creating an `Engine` in Lumen.  Here is an example `MyGui.h`:

```C++
#include "Application/LumenWindow/LumenWindow.h"

class MyGui : public LumenWindow
{
public:

    // Constructor.
    MyGui(const std::string& name, int windowFlags = 0);
    
    // Rendering functions.
    virtual void begin() override;
    virtual void onRender() override;
    virtual void end() override;
}
```

We have to inherit from `LumenWindow` that gives us all of the functionality we need to display a `GUI` in Lumen.  Al we have to do is overload the rendering functions and add the data we want.  We also have the option to add events, but this is rarely necessary for a `GUI`, since dear imgui handles the events.  Take a look at [LumenWindow.h](https://github.com/Alex-vZyl/Lumen/blob/Main/ElecDev_Graphics_Application/Source/Application/LumenWindow/LumenWindow.h).  For some information on what functions are available, take a look at [imgui.h](https://github.com/ocornut/imgui/blob/master/imgui.h) (we use the docking branch!).  An example `MyGui.cpp`:

```C++
#include "GUI/MyGui/MyGui.h"
#include "Engines/My2DEngine/My2DEngine.h"
#include "External/ImGUI/Core/imgui.h"

// Constructor.
MyGui::MyGui(const std::string& name, int windowFlags)
    : LumenWindow(name, windowflags)
{}

// The most important part of this function is calling ImGui::Begin().
// Seperating this function from the main and end function allows Lumen
// to optimise the rendering of engines and GUIs.  In addition to calling
// begin, this function should also be used to setup styles, sizes, etc.
void MyGui::begin()
{
    // Sets the size of the window (only runs on the first call).
    ImGui::SetNextWindowSize({ 400.f, 400.f }, ImGuiCond_Once);
    // Calling this function allows us to set a specific style for the window.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 1.f, 1.f });
    // Begins the ImGui window.
    ImGui::Begin(getImGuiName(), &m_isOpen, getImGuiWindowFlags());
}

// This should render the contents of the window.
void MyGui::onRender()
{
    // Create an ImGui button.  If the button is pressed the if statement will be entered.
    if(ImGui::Button("A button", { 10, 10 }))  // The second parameter is the size.
    {
        // First find the currently active engine.
        if(My2DEngine* engine = Lumen::getApp().getActiveEngine<My2DEngine>())
        {
            // Now interact with the engine!
            engine->function(...);
        }
    }
}

// The most important part of this function is calling ImGui::End().
// It should also be used to do any cleanup code necessary.
void MyGui::end()
{
    // Ends the ImGui window.
    ImGui::End();
    // Pop the style that was pushed with begin().
    ImGui::PopStyleVar();
}
```

## Spawning

Now we have a GUI with a single button.  When the button is pressed, it calls a function in `My2DEngine`.  Now we want to create an instance of the engine and also create the GUI.  `args...` is the arguments that are passed to the constructor.

```C++
// #include "Lumen.h"
// #include "Application.h"
// #include "Engines/My2DEngine/My2DEngine.h"
// #include "GUI/MyGui/MyGui.h"

// Get the application.
Application& app = Lumen::getApp();

// Create the windows inside Lumen.
My2DEngine* engine = app.pushEngine<My2DEngine>(DockPanel::Scene, "My Engine", args...);
MyGui* gui = app.pushWindow<MyGui>(DockPanel::Left, args...);
```

And now Lumen will have an `Engine` displaying, with a `Gui` that can interact with it!
