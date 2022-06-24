#pragma once
#include "glm/glm.hpp"
class Translatable2D
{
public:
    virtual void translate(const glm::vec2& translation) = 0; 
    virtual void translateTo(const glm::vec2& position) = 0;
};

class Translatable:public Translatable2D
{
public:
    virtual void translate(const glm::vec3& translation) = 0;
    virtual void translateTo(const glm::vec3& position) = 0;
};

class Rotatable {
public:
    //virtual void rotate(float degrees, const glm::vec3& rotatePoint, const glm::vec3& rotateNormal) = 0;
    virtual void rotate(float degrees, const glm::vec3& rotateNormal = { 0.f, 0.f, 1.f }) = 0;
};

class Transformable {
public:
    virtual void transform(const glm::mat4& transform) = 0;
};

class Scalable{
public:
    virtual void scale(const glm::vec3& scaleFactor) = 0;
};

class Removable {//Are all entities removable? 
public:
    virtual void remove() = 0;
    virtual void restore() = 0;
};

class Reshapable {
public:
    virtual void translateVertexAtIndex(const unsigned& localIndex, const glm::vec2& translation) = 0;
    virtual void translateVertexAtIndexTo(const unsigned& localIndex, const glm::vec2& position) = 0;
};