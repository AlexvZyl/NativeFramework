# Engines

The next step is to create an environment where we can manipulate, add and remove `Entities` contained in the `Scene`.  This is where `Engines` come in.  There are 3 `Engines` that are internal to Lumen, namely `EngineCore`, `Base2DEngine` and `Base3DEngine`.  The following examples shows how to set up a 2D engine.  An example `My2DEngine.h`:

```C++

#include "Engines/Base2DEngine/Base2DEngine.h"

class My2DEngine : public Base2DEngine
{
public:

    // Constructor.
    My2DEngine();
    // Destructor.
    inline ~My2DEngine() = default;
    
private:

    // Mouse events.
    virtual void onMouseButtonEvent(const MouseButtonEvent& event) override;
    virtual void onMouseMoveEvent(const MouseMoveEvent& event) override;
    virtual void onMouseScrollEvent(const MouseScrollEvent& event) override;
    virtual void onMouseDragEvent(const MouseDragEvent& event) override;
    // Key events.
    virtual void onKeyEvent(const KeyEvent& event) override;
    
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

void My2DEngine::onMouseMoveEvent(const MouseMoveEvent& event)
{
    // ...
}

void My2DEngine::onMouseScrollEvent(const MouseScrollEvent& event)
{
    // Using predefined controls.
    Base2DEngine::onMouseScrollEvent(event);
    
    // ...
}

void My2DEngine::onMouseDragEvent(const MouseDragEvent& event)
{
    // Using predefined controls.
    Base2DEngine::onMouseDragEvent(event);
    
    // ...
}

void My2DEngine::onMouseButtonEvent(const MouseButtonEvent& event)
{
    // ...
}

void My2DEngine::onKeyEvent(const KeyEvent& event)
{
    // ...
}
```

Now that we know how to receive events to our `Engine`, let us start using it.  It is a good idea to go look at [Events.h](https://github.com/AlexEnerdyne/Lumen/blob/Main/ElecDev_Graphics_Application/Source/Application/Events/Events.h).  If you look at `enum EventType`, you can see that we use bitshifts to create different types of `Events`.  The following example moves `m_myCircle1` to the mouse position on a left mouse button press, as well as keep our base controls.

```C++
#include "Engines/My2DEngine/My2DEngine.h"
#include "OpenGL/Entities/Circle.h"
#include "Application/Events/Events.h"

void My2DEngine::onMouseButtonEvent(const MouseButtonEvent& event)
{        
    // This checks if these two flags are contained in the ID.
    if(event.isType(EventType_MousePress | EventType_MouseButtonLeft))
    {
        // It is important to note that the mouse coordinates that are passed through the
        // event are given in window pixel coordinates.  We must first convert these coordinates
        // into our world space coordinate system.
        glm::vec3 worldSpaceCoordinates = pixelToWorldCoords(event.mousePosition);
        // Now we can use these coordinates to move our circle.
        m_myCircle1->translateTo({ worldSpaceCoordinates.x, worldSpaceCoordinates.y });
    }
}
```

## Entities

Now that we know how `Events` work, we need to be able to detect if our mouse is over a specific `Entity`.  For that Lumen has an `EntityManager` that assign entity IDs.  It is not important for the developer to know how this is done.  The following example changes the color of an entity if it is pressed:

```C++
#include "Engines/My2DEngine/My2DEngine.h"
#include "OpenGL/Entities/Circle.h"
#include "Application/Events/Events.h"
#include "Graphics/Entities/EntityManager.h"

void My2DEngine::onMouseButtonEvent(const MouseButtonEvent& event)
{    
    // This checks if those two flags are contained in the ID.
    if(event.isType(EventType_MousePress | EventType_MouseButtonLeft))
    {
        // It is important to note that the mouse coordinates that are passed through the
        // event are given in window pixel coordinates.  We must first convert these coordinates
        // into our world space coordinate system.
        glm::vec3 worldSpaceCoordinates = pixelToWorldCoords(event.mousePosition);
        
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
            entity->setColor({ 1.f, 0.f, 1.f, 1.f });    
        }
    }
}
```

TODO: Cullen to add a section on `Entity` parents.

## Spawning

`My2DEngine` is now fully functioning.  There is only one last thing to do, we need to be able to start using instances of `My2DEngine` inside of Lumen.  Creating a window with an instance can be seen below.  `args..` is the constructor arguments for the engine.  In this case there are none.

```C++
// #include "Lumen.h"
// #include "Application.h"
// #include "Engines/My2DEngine/My2DEngine.h"

Lumen::getApp().pushEngine<My2DEngine>(DockPanel::Scene, "My2DEngine Name", args...);
```

And now it will be showing in a window inside Lumen and receive events!  Lumen uses `templates` to push layers which means we do not have to change anything inside Lumen for it to be able to work with various types of `Engines`, it can display any type of custom engine any developer decides to create.  `Lumen::getApp()` is a static function that gives us a pointer to the singleton of `Application`, so this can be called from anywhere.
