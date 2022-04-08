#pragma once
#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Peripherals/Component2D.h"

enum class CompDesignState
{
    SELECT,
    DRAW_POLY,
    DRAW_LINE,
    DRAW_CIRCLE,
    PLACE_PORT
};

class ComponentDesigner : public Base2DEngine
{
protected:

public:

    std::shared_ptr<Component2D> m_activeComponent;
    Polygon2D* m_activePoly;
    LineSegment* m_activeLine;
    Circle* m_activeCircle;
    Text* m_activeText;
    std::shared_ptr<Port> m_activePort;
    VertexData* m_activeVertex;
    PortType next_port_type = PortType::PORT_INOUT;

    glm::vec2 m_lastDragPos = { 0.f, 0.f };
    unsigned int m_currentEntityID = 0;
    float clickTol = 0.01f;
    bool drawFilled = true;

    CompDesignState designerState = CompDesignState::SELECT;

    // Constructor.
    ComponentDesigner();

    void setComponent(const YAML::Node& node, Circuit* parent = nullptr);

    // Mouse events.
    virtual void onMouseButtonEvent(MouseButtonEvent& event) override;
    virtual void onMouseMoveEvent(MouseMoveEvent& event) override;
    virtual void onMouseScrollEvent(MouseScrollEvent& event) override;
    virtual void onMouseDragEvent(MouseDragEvent& event) override;
    // Key events.
    virtual void onKeyEvent(KeyEvent& event) override;
    virtual void onNotifyEvent(NotifyEvent& event) override;

    // Design palette.
    virtual void renderDesignPalette() override;

    void switchState(CompDesignState state);
    void pushActivePrimitives();
    void setActiveVertex(glm::vec2 coords);
    void setActivePrimitives(unsigned eID);
    void deleteActivePrimitive();
    // Buttons state.
    bool m_polygon = false;
    bool m_lines = false;
    bool m_delete = false;
    bool m_ports = false;
    inline void clearStates() 
    {
        m_polygon = false;
        m_lines = false;
        m_delete = false;
        m_ports = false;
    }
};

