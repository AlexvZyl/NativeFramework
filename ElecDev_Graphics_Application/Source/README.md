# Lumen Documentation

This is the non-official documentation of the Lumen Graphics Engine.  This contains information for new team members to get up and running with rendering inside of Lumen.  No knowledge of the backend is required!

Lumen has its own implementation of a graphics renderer.  We did this so that we are not limited by the API of someone else's work and so that we can change and implement whatever we want.  This turned out to be very beneficial.  For example, we render text based on a new method of Signed Distance Fields that someone developed in his Master's degree in graphics.  We would have no control over this if we had to use someone else's (high level) library.  Currently our renderer only supports an [OpenGL](https://www.opengl.org/) backend, however we have plans to implement a [Vulkan](https://www.vulkan.org/) renderer in the future.

## Prelude

We use [GLM](https://github.com/g-truc/glm) for matrices, vectors, linear algebra etc.  Some example usage:

```C++
// Vector examples.
glm::vec2 myVec2(1.f, 1.f);
glm::vec3 myVec3(1.f, 1.f, 1.f);
glm::vec4 myVec4(1.f, 1.f, 1.f, 1.f);

// Matrix examples.
glm::mat4 myMat4(1.f);  // Identity matrix.
glm::mat4 myProjectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);

// Manipulating matrices.
glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
View = glm::rotate(View, 3.f, glm::vec3(-1.0f, 0.0f, 0.0f));
View = glm::rotate(View, 4.f, glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
```

A very handy feature is the fact that glm vectors can be interpreted as position or color data:

```C++
glm::vec4 myVec4(0.1f, 0.2f, 0.3f, 0.4f);

// Position data.
float x = myVec4.x;  // 0.1f
float y = myVec4.y;  // 0.2f
float z = myVec4.z;  // 0.3f
float w = myVec4.w;  // 0.4f

// Color data.
float r = myVec4.r;  // 0.1f
float g = myVec4.g;  // 0.2f
float b = myVec4.b;  // 0.3f
float a = myVec4.a;  // 0.4f
```

[Dear ImGui](https://github.com/ocornut/imgui) is used as the GUI and they use their own vectors, namely `ImVec2` and `ImVec4`.  Lumen overloads these vectors so that they are interchangeable with `glm` vectors.  These examples are all valid:

```C++
ImVec2 vector1(0,0);
glm::vec2 vector2 = vector1;
glm::vec2 cursosPos = ImGui::GetCursosPos();  // Returns an ImVec2.
```

When creating vectors and matrices, stick to `glm`.

## The Renderer

In Lumen we have a static `Renderer` class.  This means that there is never going to be an instantation of `Renderer` (the constructor is private).  `Renderer` contains static variables and the functions make use of these variables.  So, instead of having to do this every time:

```C++
Renderer renderer(...);
renderer.function(...);
```

We simply do:

```C++
Renderer::function(...);
```

Lumen supports rendering to multiple scenes (as many as the user requires).  This is implemented by using a `Scene` class.  The contents of `Scene` is not of importance to the developer, but it contains all of the data that a graphics API requires to render a scene.  It includes things such as Vertex Array Objects, Framebuffer Objects, a camera, textures etc.  However, recall that the `Renderer` uses static variables, so this means we have to tell the `Renderer` to which `Scene` it has to render:

```C++
Scene scene(cameraType, width, height);
Renderer::bindScene(&scene);
```

When creating a `Scene` we need to specify what type of `Camera` we require, since Lumen supports 2D as well as 3D.  `width` and `height` are the dimensions of the `Scene` in pixels, but this is not strictly neccessary for the developer, since Lumen handles resizing events as well.  Binding the `Scene` tells the `Renderer` that all of the following render functions should be applied to that specific scene.  (**NOTE**:  Lumen's event system binds the scenes automatically based on certain events, so the developer actually never has to bind the `Scene`.  It is only mentioned here to give a proper description of the `Renderer` works.)

Now we want to start rendering to the `Scene`.  To see a list of the available functions, see [RendererGL.h](https://github.com/AlexEnerdyne/Lumen/blob/Main/ElecDev_Graphics_Application/Source/Graphics/OpenGL/RendererGL.h).  To render a simple circle to the `Scene`, we do this:

```C++
// Setup.
Scene scene(CameraType::Standard2D, 900, 900);
Renderer::bindScene(&scene);

// Rendering.
Renderer::addCircle2D(glm::vec3(0.f, 0.f, 0.f), 0.5f, glm::vec4(0.f, 0.f, 0.f, 1.f));

// Optional cleanup.
Renderer::unbindScene();
```

Now our `Scene` has a black circle, with radius 0.5, around the center.  However, static circles are of little use to us, so let us start manipulating it.

```C++
// Setup.
Scene scene(CameraType::Standard2D, 900, 900);
Renderer::bindScene(&scene);

// Rendering.
Circle* myCircle = Renderer::addCircle2D(glm::vec3(0.f, 0.f, 0.f), 0.5f, glm::vec4(0.f, 0.f, 0.f, 1.f));
// Manipulation.
myCircle->translate(glm::vec2(1.f, 1.f));
myCircle->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
myCircle->scale(glm::vec3(2.f, 2.f, 1.f));

// Optional cleanup.
Renderer::unbindScene();
```

Now we have a white circle, with radius 1, that is centered around (1,1).  To see a list of functions that can be used, take a look at [Primitive.h](https://github.com/AlexEnerdyne/Lumen/blob/Main/ElecDev_Graphics_Application/Source/Graphics/OpenGL/Entities/Primitive.h).  `Primitives` are described as a basic rendering shape, for example circles, polygons, text etc.  `Entities` are seen as a collection of primitives, and these have to be created by the developer.  For example, in the current `Design2DEngine` we have a `Component2D` class that stores pointers to all of the `Primitives` that make up the component.  The `Primitives` are added to the `Scene` in the constructor.  This allows you to only have to interact with a `Component2D` and not a bunch of `Primitives`.

## Creating An Engine

The next step is to create an environment where we can manipulate, add and remove `Entities` contained in the `Scene`.  This is where `Engines` come in.  There are 3 `Engines` that are internal to Lumen, namely `EngineCore`, `Base2DEngine` and `Base3DEngine`.  The following examples shows how to set up a 2D engine.  An example `My2DEngine.h`:

```C++

#include "Engines/Base2DEngine/Base2DEngine.h"

class My2DEngine : public Base2DEngine
{
public:

    // Constructor.
    My2DEngine();
    // Destructor.
    ~My2DEngine() = default;
    
    // Mouse events.
    virtual void onMouseButtonEvent(MouseButtonEvent& event) override;
    virtual void onMouseMoveEvent(MouseMoveEvent& event) override;
    virtual void onMouseScrollEvent(MouseScrollEvent& event) override;
    // Key events.
    virtual void onKeyEvent(KeyEvent& event) override;
    
private:
    
    // Example primitives.  Ideally these should be Entities created by the developer.
    Circle* m_myCircle1 = nullptr;
    Circle* m_myCircle2 = nullptr;
}

```

We created an `Engine` that includes functionality already contained inside `EngineCore` and `Base2DEngine`.  The `Base2DEngine` comes with things such as a `Scene`, event dispatcher and basic 2D camera controls.  For more information on what these classes include, take a look at the [Engines](https://github.com/AlexEnerdyne/Lumen/tree/Main/ElecDev_Graphics_Application/Source/Engines).  An example `My2DEngine.cpp`:

```C++

#include "Engines/My2DEngine/My2DEngine.h"
#include "OpenGL/RendererGL.h"
#include "OpenGL/Entities/Circle.h"

My2DEngine::My2DEngine()
    : Base2DEngine()
{
    // We do not have to bind the scene here, EngineCore does that for us.
    m_myCircle1 = Renderer::addCircle2D(...);
    m_myCircle2 = Renderer::addCircle2D(...);
}

```

### Events

We have an engine that can draw to a `Scene`, now we need to be able to handle events.  And an example `My2DEngine_Events.cpp`can be seen below.  If we want complete custom controls we should implement them here.  However, `Base2DEngine` comes with basic 2D `Camera` controls, so we can just use them.

```C++
#include "Engines/My2DEngine/My2DEngine.h"
#include "OpenGL/RendererGL.h"
#include "OpenGL/Entities/Circle.h"
#include "Application/Events/Events.h"

void My2DEngine::onMouseMoveEvent(MouseMoveEvent& event)
{
    // Using predefined controls.
    Base2DEngine::onMouseMoveEvent(event);
    
    // Implement our own logic...
}

void My2DEngine::onMouseScrollEvent(MouseScrollEvent& event)
{
    // Using predefined controls.
    Base2DEngine::onMouseScrollEvent(event);
    
    // Implement our own logic...
}

void My2DEngine::onMouseButtonEvent(MouseButtonEvent& event)
{
    // Using predefined controls.
    Base2DEngine::onMouseButtonEvent(event);
    
    // Implement our own logic...
}

void My2DEngine::onKeyEvent(KeyEvent& event)
{
    // Using predefined controls.
    Base2DEngine::onKeyEvent(event);
    
    // Implement our own logic...
}
```

Now that we know how to receive events to our `Engine`, let us start using it.  It is a good idea to go look at [Events.h](https://github.com/AlexEnerdyne/Lumen/blob/Main/ElecDev_Graphics_Application/Source/Application/Events/Events.h).  If you look at `enum EventType`, you can see that we use bitshifts to create different types of `Events`.  The following example moves `m_myCircle1` to the mouse position on a left mouse button press, as well as keep our base controls.

```C++
#include "Engines/My2DEngine/My2DEngine.h"
#include "OpenGL/Entities/Circle.h"
#include "Application/Events/Events.h"

void My2DEngine::onMouseButtonEvent(MouseButtonEvent& event)
{
    // Using predefined controls.
    Base2DEngine::onMouseButtonEvent(event);
    
    // The event ID is a description of the event, using the enum EventType.
    uint64_t eventID = event.ID;
    
    // This checks if these two flags are contained in the ID, it does NOT check
    // any of the other flags (we use an overloaded operator).
    if(eventID == (EventType_MousePress | EventType_MouseButtonLeft))
    {
        // It is important to note that the mouse coordinates that are passed through the
        // event are given in window pixel coordinates.  We must first convert these coordinates
        // into our world space coordinate system.
        glm::vec3 worldSpaceCoordinates = m_scene->pixelCoordsToWorldCoords(event.mousePosition);
        // Now we can use these coordinates to move our circle.
        m_myCircle1->translateTo(glm::vec2(worldSpaceCoordinates.x, worldSpaceCoordinates.y));
    }
}
```

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
