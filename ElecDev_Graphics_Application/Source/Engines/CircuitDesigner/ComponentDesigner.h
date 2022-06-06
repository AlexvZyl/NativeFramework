#pragma once

#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Peripherals/Port.h"
#include <filesystem>

class Component2D;
class Circuit;
class Polygon2D;
class PolyLine;
class Circle;
class Text;
enum class PortType;

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
    float clickTol = 15.0f;
    bool drawFilled = true;
    float penThickness = 0.0001f;
    int sizePt = 10;
    float textSize = sizePt / 2835.f;
    glm::vec4 penColour = { 0.f, 0.f, 0.f, 1.f };
    glm::vec4 textColour = { 0.f, 0.f, 0.f, 1.f };

    CompDesignState designerState = CompDesignState::SELECT;


    //tooltip images
    unsigned draw_clear_poly_icon;
    unsigned draw_filled_poly_icon;
    unsigned draw_clear_circle_icon;
    unsigned draw_filled_circle_icon;
    unsigned draw_text_icon;
    unsigned draw_line_icon;
    unsigned delete_icon;
    unsigned port_icon;
    unsigned colour_palette_icon;
    unsigned dropdown_icon;
    unsigned pencil_icon;

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
    virtual void renderMenuBar() override;
    virtual void renderOverlay() override;
    virtual void renderTooltip() override;

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

