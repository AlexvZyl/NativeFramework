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
