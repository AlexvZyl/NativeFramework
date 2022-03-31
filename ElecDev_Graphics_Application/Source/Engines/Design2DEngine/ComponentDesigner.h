#pragma once
#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Peripherals/Component2D.h"

enum class CompDesignState
{
    SELECT,
    DRAW_POLY,
    DRAW_LINE,
    DRAW_CIRCLE
};

class ComponentDesigner :
    public Base2DEngine
{
protected:
    

public:

    std::shared_ptr<Component2D> m_activeComponent;
    Polygon2D* m_activePoly;
    LineSegment* m_activeLine;
    Circle* m_activeCircle;
    CompDesignState designerState = CompDesignState::SELECT;

    //Constructor
    ComponentDesigner();

    // Mouse events.
    virtual void onMouseButtonEvent(MouseButtonEvent& event) override;
    virtual void onMouseMoveEvent(MouseMoveEvent& event) override;
    virtual void onMouseScrollEvent(MouseScrollEvent& event) override;
    // Key events.
    virtual void onKeyEvent(KeyEvent& event) override;

    virtual void renderDesignPalette() override;

};

