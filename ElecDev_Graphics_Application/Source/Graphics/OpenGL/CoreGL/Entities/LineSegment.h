#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Vertex.h"
#include "Polygon.h"
#include <memory>
#include <boost/utility/base_from_member.hpp>

//==============================================================================================================================================//
//  LineSegment Class.																															//
//==============================================================================================================================================//

class LineSegment : private boost::base_from_member< glm::vec2, 1>,
                    private boost::base_from_member< glm::vec2, 2>,
                    public Polygon2D
{
private:

    typedef boost::base_from_member<glm::vec2, 1> m_direction;
    typedef boost::base_from_member<glm::vec2, 2> m_perpendicular;
    float m_thickness;

public:

    // Data.
    glm::vec2 m_start;
    glm::vec2 m_end;

    // Constructor.
    LineSegment(glm::vec2 start, glm::vec2 end, VertexArrayObject<VertexData>* VAO, Entity* parent, 
                float thickness = 0.001f, glm::vec4 colour = { 0.f, 0.f, 0.f, 1.f });
    // Translate by the given vector.
    void translate(glm::vec2& translation) override;
};

//==============================================================================================================================================//
//  EOF.																															            //
//==============================================================================================================================================//