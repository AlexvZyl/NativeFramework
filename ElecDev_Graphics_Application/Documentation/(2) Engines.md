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

## Events

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
