# Lumen Documentation

*Outdated.*

This is the non-official documentation of the Lumen Graphics Engine.  This contains information for new team members to get up and running with rendering inside of Lumen.  No knowledge of the backend is required!

Lumen has its own implementation of a graphics renderer.  We did this so that we are not limited by the API of someone else's work and so that we can change and implement whatever we want.  This turned out to be very beneficial.  For example, we render text based on a new method of Signed Distance Fields that someone developed in his Master's degree in graphics.  We would have no control over this if we had to use someone else's (high level) library.  Currently our renderer only supports an [OpenGL](https://www.opengl.org/) backend, however we have plans to implement a [Vulkan](https://www.vulkan.org/) renderer in the future.

### Entities

Now that we know how `Events` work, we need to be able to detect if our mouse is over a specific `Entity`.  For that Lumen has an `EntityManager` that assign entity IDs.  It is not important for the developer to know how this is done.  The following example changes the color of an entity if it is pressed:

```C++
#include "Engines/My2DEngine/My2DEngine.h"
#include "OpenGL/Entities/Circle.h"
#include "Application/Events/Events.h"
#include "Graphics/Entities/EntityManager.h"

void My2DEngine::onMouseButtonEvent(MouseButtonEvent& event)
{
    // Using predefined controls.
    Base2DEngine::onMouseButtonEvent(event);
    
    // The event ID is a description of the event, using the enum EventType.
    uint64_t eventID = event.ID;
    
    // This checks if those two flags are contained in the ID, it does NOT check
    // any of the other flags (we use an overloaded operator).
    if(eventID == (EventType_MousePress | EventType_MouseButtonLeft))
    {
        // It is important to note that the mouse coordinates that are passed through the
        // event are given in window pixel coordinates.  We must first convert these coordinates
        // into our world space coordinate system.
        glm::vec3 worldSpaceCoordinates = m_scene->pixelCoordsToWorldCoords(event.mousePosition);
        
        // First we need to retrieve the ID of the entity that the mouse is on.
        // This is a function that we get from EngineCore.  Note that this function 
        // takes the mouse coordinate in pixels.
        unsigned entityID = getEntityID(event.mousePosition);
        
        // Now that we have the ID of the entity, we have to retrieve it
        // from the EntityManager.
        Entity* entity = EntityManager::getEntity(entityID);
        
        // We need to check if there is an entity.  If there is no entity
        // under the cursor we get a nullptr.
        if(entity)  // This is the same as 'if(entity != nullptr)'
        {
            // And now we can change the color.
            entity->setColor(glm::vec4(1.f, 0.f, 1.f, 1.f));    
        }
    }
}
```

TODO: Cullen to add a section on `Entity` parents.

### Layers

`My2DEngine` is now fully functioning.  There is only one last thing to do, we need to be able to start using instances of `My2DEngine` inside of Lumen.  Creating a window with an instance is as easy as:

```C++
// #include "Lumen.h"
// #include "Application.h"
// #include "Engines/My2DEngine/My2DEngine.h"

Lumen::getApp().pushEngineLayer<My2DEngine>("My2DEngine Name", DockPanel::Scene);
```

Pushing layers into Lumen only allows constructors that provide names and flags (flags are not important for the developer).  So if you want to have extra arguments passed into the constructor you can do something like:

```C++
// #include "Lumen.h"
// #include "Application.h"
// #include "Engines/My2DEngine/My2DEngine.h"

My2DEngine* myEnginePtr = Lumen::getApp().pushEngineLayer<My2DEngine>("My2DEngine Name", DockPanel::Scene)->getEngine();
myEnginePtr->init(...);
```

And now it will be showing in a window inside Lumen and receive events!  Lumen uses `templates` to push layers which means we do not have to change anything inside Lumen for it to be able to work with various types of `Engines`, it can display any type of custom engine any developer decides to create.  `Lumen::getApp()` is a static function that gives us a pointer to the singleton of `Application`, so this can be called from anywhere inside Lumen.

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
