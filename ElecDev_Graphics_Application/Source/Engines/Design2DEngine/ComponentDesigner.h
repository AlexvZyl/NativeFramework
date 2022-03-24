#pragma once
#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Peripherals/Component2D.h"
class ComponentDesigner :
    public Base2DEngine
{
protected:
    

public:

    std::shared_ptr<Component2D> m_activeComponent;
    //Constructor
    ComponentDesigner();

    // Mouse events.
    virtual void onMouseButtonEvent(MouseButtonEvent& event) override;
    virtual void onMouseMoveEvent(MouseMoveEvent& event) override;
    virtual void onMouseScrollEvent(MouseScrollEvent& event) override;
    // Key events.
    virtual void onKeyEvent(KeyEvent& event) override;

};

