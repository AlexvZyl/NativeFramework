## Creating A GUI

Creating a `GUI` is very similar to creating an `Engine` in Lumen.  Here is an example `MyGui.h`:

```C++
#include "GUI/GuiElementCore/GuiElementCore.h"

class MyGui : public GuiElementCore
{
public:

    // Constructor.
    MyGui(std::string& name, int windowFlags = 0);
    
    // Set the engine that the GUI interacts with.
    void setEngine(My2DEngine* engine);
    
    // Rendering functions.
    virtual void begin() override;
    virtual void onRender() override;
    virtual void end() override;
    
private:
    
    // We want to GUI to be able to interact with our engine.
    My2DEngine* m_engine = nullptr;
}
```

We have to inherit from `GuiElementCore` that gives us all of the functionality we need to display a `GUI` in Lumen.  Al we have to do is overload the rendering functions and add the data we want.  We also have the option to add events, but this is rarely necessary for a `GUI`, since dear imgui handles the events.  Take a look at [GuiElementCore.h](https://github.com/Alex-vZyl/Lumen/blob/Main/ElecDev_Graphics_Application/Source/GUI/GuiElementCore/GuiElementCore.h).  For some information on what functions are available, take a look at [imgui.h](https://github.com/ocornut/imgui/blob/master/imgui.h).  An example `MyGui.cpp`:

```C++
#include "GUI/MyGui/MyGui.h"
#include "Engines/My2DEngine/My2DEngine.h"
#include "External/ImGUI/Core/imgui.h"

// Constructor.

MyGui::MyGui(std::string& name, int windowFlags)
    : GuiElementCore(name, windowflags)
{
    // We cannot pass more arguments into the contructor (allows us to push layers with templates).
    // So we use helper functions.
}

// This is a function we can call after the constructor to set data that
// we did not set in the constructor.
void MyGui::setEngine(My2DEngine* engine)
{
    m_engine = engine;
}

// The most important part of this function is calling ImGui::Begin().
// Seperating this function from the main and end function allows Lumen
// to optimise the rendering of engines and GUIs.  In addition to calling
// begin, this function should also be used to setup styles, sizes, etc.
void MyGui::begin()
{
    // Sets the size of the window (only runs on the first call).
    ImGui::SetNextWindowSize(glm::vec2{ 400.f, 400.f }, ImGuiCond_Once);
    // Calling this function allows us to set a specific style for the window.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, glm::vec2(1.f, 1.f));
    // Begins the ImGui window.
    ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

// This should render the contents of the window.
void MyGui::onRender()
{
    // Create an ImGui button.  If the button is pressed the if statement will be entered.
    if(ImGui::Button("A button", glm::vec2(10, 10)))  // The second parameter is the size.
    {
        // Call a function in the engine when a button is pressed.
        m_engine->function(...);
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

### Layers

Now we have a GUI with a single button.  When the button is pressed, it calls a function in `My2DEngine`.  Now we want to create an instance of the engine and also create the GUI:

```C++
// #include "Lumen.h"
// #include "Application.h"
// #include "Engines/My2DEngine/My2DEngine.h"
// #include "GUI/MyGui/MyGui.h"

// Get the application.
Application& app = Lumen::getApp();

// Create the windows inside Lumen.
My2DEngine* engine = app.pushEngineLayer<My2DEngine>("My Engine", DockPanel::Scene)->getEngine();
MyGui* gui = app.pushGuiLayer<MyGui>("My Gui", DockPanel::Left)->getGui();

// Now some setup.
gui->setEngine(engine);
```

And now Lumen will have an `Engine` displaying, with a `Gui` that can interact with it!
