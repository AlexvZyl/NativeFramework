#pragma once
#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Peripherals/Component2D.h"
#include <filesystem>

enum class CompDesignState
{
    SELECT,
    DRAW_POLY,
    DRAW_LINE,
    DRAW_CIRCLE,
    PLACE_PORT,
    ADD_TEXT
};

class ComponentDesignerColorEditor;

class ComponentDesigner : public Base2DEngine
{
protected:

public:

    std::shared_ptr<Component2D> m_activeComponent;
    Polygon2D* m_activePoly;
    PolyLine* m_activeLine;
    Circle* m_activeCircle;
    Text* m_activeText;
    std::shared_ptr<Port> m_activePort;
    //VertexData* m_activeVertex;
    unsigned m_activeVertexIdx = -1;
    PortType next_port_type = PortType::PORT_INOUT;

    glm::vec2 m_lastDragPos = { 0.f, 0.f };
    unsigned int m_currentEntityID = 0;
    float clickTol = 0.03f;
    bool drawFilled = true;
    float penThickness = 0.014f;

    CompDesignState designerState = CompDesignState::SELECT;

    // Constructor.
    ComponentDesigner();

    void setComponent(const std::filesystem::path& path, Circuit* parent = nullptr);

    // Mouse events.
    virtual void onMouseButtonEvent(const MouseButtonEvent& event) override;
    virtual void onMouseMoveEvent(const MouseMoveEvent& event) override;
    virtual void onMouseScrollEvent(const MouseScrollEvent& event) override;
    virtual void onMouseDragEvent(const MouseDragEvent& event) override;
    // Key events.
    virtual void onKeyEvent(const KeyEvent& event) override;
    virtual void onNotifyEvent(const NotifyEvent& event) override;

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

